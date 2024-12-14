#include "Cli.h"
#include "FileSaver.h"
#include "globals.h"

int main() {
    globalSaver = new FileSaver("results.log");

    CTree* tree = new CTree();
    CError* error = new CError(ErrorCode::ERROR_DIVISION_BY_ZERO, ErrorLevel::CRITICAL, "Testowy błąd");

    // Utworzenie rezultatu z błędem
    std::vector<CError*> errors;
    errors.push_back(error);
    CResult<CTree*, CError> resultWithErrors(tree, errors);

    CLI cli;
    cli.run();
    delete globalSaver;
    return 0;
}
