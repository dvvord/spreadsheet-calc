#include "worker_node.h"

using namespace std;

const regex WorkerNode::pattern = regex("[A-Z][1-9][0-9]*");

void WorkerNode::parseFormulaForDeps(string formula, vector<Key>& deps) const {  
    
    smatch results;
    regex_match (formula,results,pattern);
    string::const_iterator searchStart( formula.cbegin() );
    while ( regex_search( searchStart, formula.cend(), results, pattern ) ) {
        string res(results[0]);
        char c = res[0];
        int n = atoi(res.substr(1,res.size()).c_str());
        auto p = make_pair(c,n);
        deps.emplace_back(p);
        searchStart += results.position() + results.length();
    }
}

bool WorkerNode::isOperation(char b) const {
    return b=='+' || b=='-' || b=='*' || b=='/' ; 
}

float WorkerNode::evaluate_posfix ( const std::string& expression ) const {
    float l=0.0,r=0.0,ans=0.0;
    float result = 0;
    stringstream postfix(expression);
    vector<float> stack;
    string s;
    while ( postfix >> s ) {
        if( isOperation(s[0]) && stack.size()>=2 && (s[1]==' ' || s[1]=='\0')) {
            r = stack.back();
            stack.pop_back();
            l = stack.back();
            stack.pop_back();
            switch( s[0]) {
                case '+': ans =  l + r ; break;
                case '-': ans =  l - r ; break;
                case '*': ans =  l * r ; break;
                case '/':
                    if(r==0) throw exception();
                    ans =  l / r ; 
                    break;
                default:
                    throw invalid_argument( "incorrect format of the value" );
            }
            stack.push_back( ans );
        }
        else {
            stack.push_back(stof(s));
        }
    }
    if(stack.size()>0)
        result = stack.back();
    else {
        throw invalid_argument( "incorrect format of the value" );
    }

    return result;
} 

bool WorkerNode::preprocessFormula(string & formula) const
{
    bool result = false;
    formula+=" "; 
    while((result = replaceAll(formula," -- "," 1 - ")));
    while((result = replaceAll(formula," ++ "," 1 + ")));
    while((result = replaceAll(formula,"++","")));
    while((result = replaceAll(formula,"--","")));
    return result;
}

bool WorkerNode::evaluateFormula(string formula, float& result) const {
    bool res = true;
    try { 
        preprocessFormula(formula);
        result = evaluate_posfix(formula);
    }
    catch(const exception&) {
        Message terminate{ make_pair(0,0),"",float(Error::FORMAT),MessageType::TERMINATE };
        input_queue->send(terminate);
        res = false;        
    } 
    return res;
}

void WorkerNode::parseFormula(Message& m) {
    vector<Key> deps;

    parseFormulaForDeps(m.formula,deps);
    unsigned int cached_deps = 0;
    for(const auto & d : deps) {
        if(spreadsheet->hasValue(d)) {
            cached_deps++;
        }
        else { // not all value for dependencies are available
            vector<int> ids;
            waiting_list->get(d, ids);
            if (find(ids.begin(), ids.end(), id) == ids.end()) {
                waiting_list->push_back(d,id);
            }
            auto & dep_list = deps_cache[d];
            if (find(dep_list.begin(), dep_list.end(), m.key) == dep_list.end()) {
                dep_list.push_back(m.key);
            }
        }
    }

    if(cached_deps==deps.size() || deps.size()==0) { // we have values for all deps
        string formula = m.formula;
        for(auto & d : deps) {
            string pattern = d.first+to_string(d.second);
            float v = spreadsheet->getValue(d);
           
            replaceAll(formula,pattern,to_string(v));
        }
        float result;
        if(evaluateFormula(formula,result)) {
            Message mm {m.key,"",result,MessageType::VALUE};
            spreadsheet->setValue(m.key,mm.value);     
            input_queue->send(mm);
        }
    }
    else { // save formula for future processing
        formula_cache[m.key]=m.formula;
    }
}

void WorkerNode::execute(Message& m)
{
    if (m.type == MessageType::FORMULA) {
        parseFormula(m);
    }
    else if (m.type == MessageType::VALUE && deps_cache.count(m.key) > 0) {
        if (!spreadsheet->hasValue(m.key)) {
            spreadsheet->setValue(m.key, m.value);
        }
        for (const auto & d : deps_cache[m.key]) {
            string f = formula_cache[d];
            Message m = Message{ d,f,0.0,MessageType::FORMULA };
            parseFormula(m);
        }

        auto elem = deps_cache.find(m.key);
        if(elem!=deps_cache.end())
            deps_cache.erase(elem);
    }
    else if (m.type == MessageType::VALUE) {
        if (!spreadsheet->hasValue(m.key)) {
            spreadsheet->setValue(m.key, m.value);
        }
    }
    else if (m.type == MessageType::TERMINATE) {
        continue_execution = false;
    }
}
void WorkerNode::run(int thread_id)
{
    id = thread_id;
   
    while(continue_execution) {
        Message m;
        message_queue->recieve(m);

        execute(m);
    }
}