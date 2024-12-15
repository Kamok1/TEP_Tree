#include "Cli.h"
#include "FileSaver.h"
#include "Globals.h"
#include "CMySmartPointer.h"
#define ERRORS_PATH "errors.log"

int main() {
    globalSaver = new FileSaver(ERRORS_PATH);
    //CMySmartPointer<CTree> tree(new CTree);
    //std::istringstream iss("+ 1 2");
    //tree->buildTree(iss);
    //CError* error = new CError(ErrorCode::ERROR_DIVISION_BY_ZERO, ErrorLevel::CRITICAL, "TEST");

    //std::vector<CError*> errors;
    //errors.push_back(error);
    //CResult<CTree*, CError> resultWithErrors(tree.get(), errors);

    CLI cli;
    cli.run();
    delete globalSaver;
    return 0;
}
