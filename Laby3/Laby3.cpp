#include "Cli.h"
#include "CResultFileSaver.h"
#include "Globals.h"
#define ERRORS_PATH "errors.log"

int main() {
    saverMangager = new SaverManager();
    saverMangager->registerSaver(new CResultFileSaver<CTree*>("tree_results.txt"));
    saverMangager->registerSaver(new CResultFileSaver<void*>("generic_results.txt"));

    CTree* tree = new CTree();
    std::istringstream iss("+ 1 2");
    tree->buildTree(iss);
    CError* error = new CError(ErrorCode::ERROR_DIVISION_BY_ZERO, ErrorLevel::CRITICAL, "TEST");

    std::vector<CError*> errors;
    errors.push_back(error);
    CResult<CTree*, CError> resultWithErrors(tree, errors);


    int error1 = 1;

    std::vector<int*> errors2;
    errors2.push_back(&error1);
    CResult<CTree*, int> resultWithErrors2(tree, errors2);


    CLI cli;
    cli.run();
    return 0;
}
