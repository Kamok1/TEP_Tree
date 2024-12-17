#include "Cli.h"
#include "CResultFileSaver.h"
#include "Globals.h"
#include "CMySmartPointer.h"
#include <iostream>
#include <sstream>
#include <vector>

#define TREE_ERRORS_FILE "tree_errors.log"
#define GENERAL_ERRORS_FILE "general_errors.log"
#define DEFAULT_ERROR_HANDLER new CResultFileSaver<void*>(GENERAL_ERRORS_FILE)


#pragma region TESTS

    void testErrorHandling() {
        std::cout << "\nTest obsługi błędów i zapisów:\n";

        CTree* tree = new CTree();
        std::istringstream iss3("+ 1 2");
        tree->buildTree(iss3);
        CError* error = new CError(ErrorCode::ERROR_DIVISION_BY_ZERO, ErrorLevel::CRITICAL, "TEST");
        std::vector<CError*> errors;
        errors.push_back(error);
        CResult<CTree*, CError> resultWithErrors(tree, errors);

        int* value = new int(42);

        CError* error2 = new CError(ErrorCode::ERROR_INVALID_INPUT, ErrorLevel::WARNING, "TEST2");
        std::vector<CError*> errors2;
        errors2.push_back(error2);

        CResult<int*, CError> resultWithErrors2(value, errors2);       
    }

    void printTree(const CTree& tree) {
        std::vector<std::string> values;
        tree.getTreeNodeValues(values);
        for (const auto& value : values) {
            std::cout << value << " ";
        }
        std::cout << std::endl;
    }

    void testTreeOperations() {
        CTree tree1;
        std::istringstream iss1("+ 1 2");
        tree1.buildTree(iss1);
        std::cout << "Oryginalne drzewo: ";
        printTree(tree1);

        CTree tree2(tree1);
        std::cout << "Skopiowane drzewo: ";
        printTree(tree2);

        CTree tree3 = tree1;
        std::cout << "Przypisane drzewo: ";
        printTree(tree3);

        CTree tree4(std::move(tree1));
        std::cout << "Drzewo przeniesione (tree4): ";
        printTree(tree4);
        std::cout << "Oryginalne drzewo po przeniesieniu (tree1): ";
        printTree(tree1);

        CTree tree5;
        tree5 = std::move(tree2);
        std::cout << "Drzewo przeniesione (tree5): ";
        printTree(tree5);
        std::cout << "Oryginalne drzewo po przeniesieniu (tree2): ";
        printTree(tree2);

        CTree tree6;
        std::istringstream iss2("10 20 +");
        tree6.buildTree(iss2);
        std::cout << "Drzewo 1: ";
        printTree(tree5);

        std::cout << "Drzewo 2: ";
        printTree(tree6);

        CTree tree7 = tree5 + tree6;
        std::cout << "Połączone drzewo: ";
        printTree(tree7);
    }

    void testSmartPointer() {
        int* ptr = new int(10);
        CMySmartPointer<int> sp1(ptr);
        std::cout << "Wartość początkowa: " << *sp1 << "\n";
        *sp1 = 20;
        std::cout << "Zmieniona wartość: " << *sp1 << "\n";

        CMySmartPointer<int> sp2(sp1);
        std::cout << "Wartość kopiowanego wskaźnika: " << *sp2 << "\n";

        CMySmartPointer<int> sp3;
        sp3 = sp1;
        std::cout << "Wartość po przypisaniu kopiującym: " << *sp3 << "\n";

        CMySmartPointer<int> sp4(std::move(sp1));
        std::cout << "Wartość po przeniesieniu: " << *sp4 << "\n";

        CMySmartPointer<int> sp5;
        sp5 = std::move(sp2);
        std::cout << "Wartość po przypisaniu przenoszącym: " << *sp5 << "\n";

        {
            CMySmartPointer<int> sp6(new int(50));
            std::cout << "Wartość wewnątrz bloku: " << *sp6 << "\n";
        }

        CMySmartPointer<int> sp9(new int(200));
        {
            CMySmartPointer<int> sp10 = sp9;
            std::cout << "Wartość wskaźnika w sp10: " << *sp10 << "\n";
            std::cout << "Wartość wskaźnika w sp9: " << *sp9 << "\n";
        }
        std::cout << "Wartość wskaźnika w sp9 po zniszczeniu sp10: " << *sp9 << "\n";
    }

    void testRefCounter() {
        CRefCounter<int> counter;
        std::cout << "Początkowa liczba referencji: " << counter.getRefCount() << "\n";

        counter.addRef();
        std::cout << "Liczba referencji po dodaniu: " << counter.getRefCount() << "\n";

        counter.addRef();
        std::cout << "Liczba referencji po kolejnym dodaniu: " << counter.getRefCount() << "\n";

        counter.releaseRef();
        std::cout << "Liczba referencji po usunięciu jednej: " << counter.getRefCount() << "\n";

        counter.releaseRef();
        std::cout << "Liczba referencji po kolejnym usunięciu: " << counter.getRefCount() << "\n";

        int result = counter.releaseRef();
        std::cout << "Liczba referencji po ostatnim usunięciu: " << result << "\n";
        if (result == 0) {
            std::cout << "Licznik poprawnie wynosi zero, można zwolnić zasoby.\n";
        }
    }

    void testAdvancedSmartPointer() {
        CMySmartPointer<int> spMain(new int(500));
        std::cout << "Wartość spMain (początkowa): " << *spMain << "\n";

        {
            CMySmartPointer<int> spCopy1 = spMain;
            CMySmartPointer<int> spCopy2 = spMain;

            std::cout << "Wartość spCopy1: " << *spCopy1 << "\n";
            std::cout << "Wartość spCopy2: " << *spCopy2 << "\n";

            *spCopy1 = 600;

            std::cout << "Po zmianie przez spCopy1:\n";
            std::cout << "Wartość spMain: " << *spMain << "\n";
            std::cout << "Wartość spCopy2: " << *spCopy2 << "\n";
        }
        std::cout << "Wartość spMain po zniszczeniu spCopy1 i spCopy2: " << *spMain << "\n";

        {
            CMySmartPointer<int> spMove = std::move(spMain);
            std::cout << "Wartość spMove (przeniesienie spMain): " << *spMove << "\n";

            if (spMain.get() == nullptr) {
                std::cout << "spMain jest teraz pusty (nullptr)\n";
            }

            CMySmartPointer<int> spCopy3 = spMove;
            std::cout << "Wartość spCopy3 (kopiowanie z spMove): " << *spCopy3 << "\n";
        }
        if (spMain.get() == nullptr) {
            std::cout << "spMain nadal jest pusty (nullptr)\n";
        }
    }

    void testTreeConsturctors()
    {
        std::cout << "Test konstruktora kopiującego:\n";
        CTree tree1;
        CTree tree2(tree1);
        CTree::printStats();

        std::cout << "\nTest operatora przypisania kopiującego:\n";
        CTree tree3;
        tree3 = tree1;
        CTree::printStats();

        std::cout << "\nTest konstruktora przenoszącego:\n";
        CTree tree4(std::move(tree1));
        CTree::printStats();

        std::cout << "\nTest operatora przypisania przenoszącego:\n";
        CTree tree5;
        tree5 = std::move(tree2);
        CTree::printStats();

        std::cout << "\nTest operatora +:\n";
        CTree tree6;
        CTree tree7 = tree2 + tree6;
        CTree::printStats();
    }
#pragma endregion



int main() {
    saverMangager = new SaverManager();
    saverMangager->registerSaver(DEFAULT_ERROR_HANDLER);
    saverMangager->registerSaver(new CResultFileSaver<CTree*>(TREE_ERRORS_FILE));


#ifdef _DEBUG
    testErrorHandling();
    testTreeConsturctors();
    testTreeOperations();
    testSmartPointer();
    testRefCounter();
    testAdvancedSmartPointer();
#endif

    CLI cli;
    cli.run();

    return 0;
}
