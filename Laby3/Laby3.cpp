#include "Cli.h"
#include "FileSaver.h"

int main() {
    globalSaver = new FileSaver("errors.log");
    CLI cli;
    cli.run();
    delete globalSaver;
    return 0;
}
