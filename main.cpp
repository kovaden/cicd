#include "Document.h"
#include "controller/Controller.h"
#include "model/Coord.h"
#include "view/View.h"

using namespace std;

Controller theController;

void onDocumentCreate() {
    theController.create_document();
}

void onDocumentOpen(const std::string &filename) {
    theController.Import(filename);
}

void onDocumentSave(const std::string &filename) {
    theController.Export(filename);
}

void onNewPoint(Coord pos) {
    theController.CreatePrimitive(Controller::pPoint, pos);
}

void onNewLine(Coord start, Coord end) {
    theController.CreatePrimitive(Controller::pLine, std::make_pair(start, end));
}

void onNewRectangle(Coord leftTop, Coord rightBottom) {
    theController.CreatePrimitive(Controller::pRectangle, std::make_pair(leftTop, rightBottom));
}

void onNewCircle(Coord center, int radius) {
    theController.CreatePrimitive(Controller::pCircle, std::make_pair(center, radius));
}

/*
 *  Find a primitive on coordinates pos and delete them with a user's permission
 */
void onDeletePrimitive(Coord pos) {
    try {
        auto found = theController.FindPrimitives(pos);

        for (const auto& p : found) {

            //TODO: Ask user if he really wants to delete this primitive
            theController.DeletePrimitive(p);
        }

    } catch (PrimitiveNotFound &x) {
        // Something went wrong
    }
}

enum UserCommand {ImportDocument, ExportDocument, Create, Find, Delete};

UserCommand getCommand() {
    throw NotImplemened();
}

int main()
{
    Controller cntrl;
    auto document = cntrl.create_document();

    string some_filename;
    Controller::PrimitiveType what{Controller::pPoint};
    Coord where;

    vector<primitive_ptr> selection;
    // Simulate GUI event loop. Normally this is a part of used GUI library which is not the case now.
    //   Just call controller on some events
    while (auto cmd = getCommand()) {

        switch (cmd) {
            case ImportDocument:
                cntrl.Import(some_filename);
                break;
            case ExportDocument:
                cntrl.Export(some_filename);
                break;
            case Create:
                cntrl.CreatePrimitive(what, Controller::CreateParams());
                break;
            case Find:
                selection = cntrl.FindPrimitives(where);
                break;
            case Delete:
                for (const auto& victim : selection) {
                    cntrl.DeletePrimitive(victim);
                }
                break;
        }

        cntrl.Draw();
    }

    return 0;
}