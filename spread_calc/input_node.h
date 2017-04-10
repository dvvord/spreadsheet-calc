#include "util.h"
#include "spreadsheet.h"
#include "basic_node.h"

class InputNode :  public BasicInput
{
public:
    void run(int row, int col);
private:
    int trial_state_counter = 0;
    int size = 0;
    void NotifyCalculated();
    void sendToWorker(int workerId, Message& m);
};