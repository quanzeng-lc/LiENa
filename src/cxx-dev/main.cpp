#include <QApplication>
#include "pissImageProcessingFactory.h"
#include "pissMainWindow.h"
#include "pissController.h"
#include "pissDataware.h"
#include "vtkOutputWindow.h"
#include "pissOmega3.h"
#include "lienaDistributedSystemInformation.h"

#define COM_ONLY 0


int main(int argc, char *argv[]){

    // création de l'application
    QApplication analyser(argc, argv);
    analyser.setOrganizationName("CAS - SIAT");
    analyser.setApplicationName("medsight");
    analyser.setWindowIcon(QIcon(":/images/icon.png"));
    vtkOutputWindow::SetGlobalWarningDisplay(0);

    // Initialiser l'environment du réseaux interne pour contruire le système réparti
    liena *communicationStack = new liena();
    communicationStack->setLocalDeviceID(SIAT_COCKPIT_VERSION_1);

    if(COM_ONLY){
        communicationStack->launch();
        communicationStack->openSessionRequest(SIAT_COCKPIT_VERSION_1, "172.20.10.2");
    }
    else{
        // le bibliothéque des algorithmes pour faire le traitement des images
        pissImageProcessingFactory *imageProcessingFactory = new pissImageProcessingFactory();

        // le base de données pour enregistrer les informations qui concerne des maladies
        pissDataware* database = new pissDataware();
        database->setImageProcessingFactory(imageProcessingFactory);

        // omega peripherique pour donner les commandesa
        pissOmega3 *omega = new pissOmega3();

        // l'ordonnanceur du système
        pissController* controller = new pissController();
        controller->setSystemDataBase(database);
        controller->setImageProcessingFactory(imageProcessingFactory);
        controller->setCommunicationStack(communicationStack);
        controller->setOmega3(omega);

        // l'IHM
        pissMainWindow* mainWindow = new pissMainWindow(controller);

        // Chercher les maladies existants locale
        mainWindow->findPatientExisted();

        // Lancer l'IHM principale
        mainWindow->display();
    }

    return analyser.exec();
}

