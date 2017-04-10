#include "util.h"
#include "spreadsheet.h"
#include "basic_node.h"

class OutputNode : public BasicNode {
public:
    void run(int row, int col);
    Key& getTargetKey() { return target_key; }
    void terminate() {
        continue_execution = false;
        auto & key = getTargetKey();
        float v = 0.0;
        spreadsheet->setValue(key, v);
    }
private:
    Key target_key = make_pair(0, 0);
};