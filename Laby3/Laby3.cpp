#include "Cli.h"
#include "CResultFileSaver.h"
#include "Globals.h"
#define TREE_ERRORS_FILE "tree_results.txt"
#define GENERAL_ERRORS_FILE "general_errors.txt"

//void printTree(const CTree& tree) {
//    std::vector<std::string> values;
//    tree.getTreeNodeValues(values);
//    for (const auto& value : values) {
//        std::cout << value << " ";
//    }
//    std::cout << std::endl;
//}

int main() {
    saverMangager = new SaverManager();
    saverMangager->registerSaver(new CResultFileSaver<CTree*>(TREE_ERRORS_FILE));
    saverMangager->registerSaver(new CResultFileSaver<void*>(GENERAL_ERRORS_FILE));

    //CTree* tree = new CTree();
    //std::istringstream iss3("+ 1 2");
    //tree->buildTree(iss3);
    //CError* error = new CError(ErrorCode::ERROR_DIVISION_BY_ZERO, ErrorLevel::CRITICAL, "TEST");
    //std::vector<CError*> errors;
    //errors.push_back(error);
    //CResult<CTree*, CError> resultWithErrors(tree, errors);
    //int error1 = 1;
    //std::vector<int*> errors2;
    //errors2.push_back(&error1);
    //CResult<CTree*, int> resultWithErrors2(tree, errors2);
    //std::vector<CError*> errors3 = saverMangager->getErrors();

    //std::cout << "Test konstruktora kopiującego i przypisania:\n";
    //CTree tree1;
    //std::istringstream iss("+ 1 2");
    //tree1.buildTree(iss);
    //std::cout << "Oryginalne drzewo: ";
    //printTree(tree1);

    //CTree tree2(tree1);
    //std::cout << "Skopiowane drzewo: ";
    //printTree(tree2);

    //CTree tree3;
    //tree3 = tree1;
    //std::cout << "Przypisane drzewo: ";
    //printTree(tree3);

    //std::cout << "\nTest konstruktora przenoszącego i przypisania przenoszącego:\n";
    //CTree tree4(std::move(tree1));
    //std::cout << "Drzewo przeniesione (tree4): ";
    //printTree(tree4);
    //std::cout << "Oryginalne drzewo po przeniesieniu (tree1): ";
    //printTree(tree1);

    //CTree tree5;
    //tree5 = std::move(tree2); 
    //std::cout << "Drzewo przeniesione (tree5): ";
    //printTree(tree5);
    //std::cout << "Oryginalne drzewo po przeniesieniu (tree2): ";
    //printTree(tree2);

    //std::cout << "\nTest operatora +:\n";
    //CTree tree6;
    //std::istringstream iss2("10 20 +");
    //tree6.buildTree(iss2);
    //std::cout << "Drzewo 1: ";
    //printTree(tree5);

    //std::cout << "Drzewo 2: ";
    //printTree(tree6);

    //CTree tree7 = tree5 + tree6; 
    //std::cout << "Połączone drzewo: ";
    //printTree(tree7);

    CLI cli;
    cli.run();

    return 0;
}
