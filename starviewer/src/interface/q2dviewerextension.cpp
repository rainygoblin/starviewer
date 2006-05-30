/***************************************************************************
 *   Copyright (C) 2005 by Grup de Gr�fics de Girona                       *
 *   http://iiia.udg.es/GGG/index.html?langu=uk                            *
 *                                                                         *
 *   Universitat de Girona                                                 *
 ***************************************************************************/
#include "q2dviewerextension.h"

#include "volume.h"
#include "logging.h"
#include "qcustomwindowleveldialog.h"
#include <QAction>
#include <QToolBar>
// VTK
#include <vtkRenderer.h>

namespace udg {

Q2DViewerExtension::Q2DViewerExtension( QWidget *parent )
 : QWidget( parent )
{
    setupUi( this );
    m_mainVolume = 0;
    m_secondaryVolume = 0;
    
    m_customWindowLevelDialog = new QCustomWindowLevelDialog;
    
    createActions();
    createToolBars();
    createConnections();
}

Q2DViewerExtension::~Q2DViewerExtension()
{
}

void Q2DViewerExtension::createActions()
{
    m_axialViewAction = new QAction( 0 );
    m_axialViewAction->setText( tr("&Axial View") );
    m_axialViewAction->setShortcut( tr("Ctrl+A") );
    m_axialViewAction->setStatusTip( tr("Change Current View To Axial") );
    m_axialViewAction->setIcon( QIcon(":/images/axial.png") );
    m_axialViewToolButton->setDefaultAction( m_axialViewAction );
    
    m_sagitalViewAction = new QAction( 0 );
    m_sagitalViewAction->setText( tr("&Sagital View") );
    m_sagitalViewAction->setShortcut( tr("Ctrl+S") );
    m_sagitalViewAction->setStatusTip( tr("Change Current View To Sagital") );
    m_sagitalViewAction->setIcon( QIcon(":/images/sagital.png") );
    m_sagitalViewToolButton->setDefaultAction( m_sagitalViewAction );
    
    m_coronalViewAction = new QAction( 0 );
    m_coronalViewAction->setText( tr("&Coronal View") );
    m_coronalViewAction->setShortcut( tr("Ctrl+C") );
    m_coronalViewAction->setStatusTip( tr("Change Current View To Coronal") );
    m_coronalViewAction->setIcon( QIcon(":/images/coronal.png") );
    m_coronalViewToolButton->setDefaultAction( m_coronalViewAction );

    m_singleViewAction = new QAction( 0 );
    m_singleViewAction->setText( tr("&Single View") );
    m_singleViewAction->setShortcut( tr("Ctrl+S") );
    m_singleViewAction->setStatusTip( tr("Change To Single View Mode") );
    m_singleViewAction->setIcon( QIcon(":/images/singleView.png") );
    m_singleViewToolButton->setDefaultAction( m_singleViewAction );

    m_doubleViewAction = new QAction( 0 );
    m_doubleViewAction->setText( tr("&Double View") );
    m_doubleViewAction->setShortcut( tr("Ctrl+D") );
    m_doubleViewAction->setStatusTip( tr("Change To Double View Mode") );
    m_doubleViewAction->setIcon( QIcon(":/images/addViewRight.png") );
    m_doubleViewToolButton->setDefaultAction( m_doubleViewAction );
}

void Q2DViewerExtension::createToolBars()
{
    m_toolsToolBar = new QToolBar(0);
    m_toolsToolBar->addAction( m_axialViewAction );
    m_toolsToolBar->addAction( m_sagitalViewAction );
    m_toolsToolBar->addAction( m_coronalViewAction );
    
}

void Q2DViewerExtension::createConnections()
{
    connect( m_slider , SIGNAL( valueChanged(int) ) , m_spinBox , SLOT( setValue(int) ) );
    connect( m_spinBox , SIGNAL( valueChanged(int) ) , m_2DView , SLOT( setSlice(int) ) );
    connect( m_2DView , SIGNAL( sliceChanged(int) ) , m_slider , SLOT( setValue(int) ) );

    connect( m_customWindowLevelDialog , SIGNAL( windowLevel( double,double) ) , m_2DView , SLOT( setWindowLevel( double , double ) ) );
    
    // adicionals, \TODO ara es fa "a saco" per� s'ha de millorar
    connect( m_slider2_1 , SIGNAL( valueChanged(int) ) , m_spinBox2_1 , SLOT( setValue(int) ) );
    connect( m_spinBox2_1 , SIGNAL( valueChanged(int) ) , m_2DView2_1 , SLOT( setSlice(int) ) );
    connect( m_2DView2_1 , SIGNAL( sliceChanged(int) ) , m_slider2_1 , SLOT( setValue(int) ) );

    connect( m_slider2_2 , SIGNAL( valueChanged(int) ) , m_spinBox2_2 , SLOT( setValue(int) ) );
    connect( m_spinBox2_2 , SIGNAL( valueChanged(int) ) , m_2DView2_2 , SLOT( setSlice(int) ) );
    connect( m_2DView2_2 , SIGNAL( sliceChanged(int) ) , m_slider2_2 , SLOT( setValue(int) ) );

    // sincronisme window level \TODO veure perqu� no hi ha sincronisme entre una p�gina i l'altre
    connect( m_2DView , SIGNAL( windowLevelChanged( double , double ) ) , m_2DView2_1 , SLOT( setWindowLevel( double , double ) ) );
    connect( m_2DView2_1 , SIGNAL( windowLevelChanged( double , double ) ) , m_2DView2_2 , SLOT( setWindowLevel( double , double ) ) );
    connect( m_2DView2_1 , SIGNAL( windowLevelChanged( double , double ) ) , m_2DView , SLOT( setWindowLevel( double , double ) ) );
    connect( m_2DView2_2 , SIGNAL( windowLevelChanged( double , double ) ) , m_2DView2_1 , SLOT( setWindowLevel( double , double ) ) );
    connect( m_2DView2_2 , SIGNAL( windowLevelChanged( double , double ) ) , m_2DView , SLOT( setWindowLevel( double , double ) ) );
    
    connect( m_axialViewAction , SIGNAL( triggered() ) , this , SLOT( changeViewToAxial() ) );
    connect( m_sagitalViewAction , SIGNAL( triggered() ) , this , SLOT( changeViewToSagital() ) );
    connect( m_coronalViewAction , SIGNAL( triggered() ) , this , SLOT( changeViewToCoronal() ) );

    connect( m_singleViewAction , SIGNAL( triggered() ) , this , SLOT( changeViewToSingle() ) );
    connect( m_doubleViewAction , SIGNAL( triggered() ) , this , SLOT( changeViewToDouble() ) );
    
    connect( m_windowLevelComboBox , SIGNAL( activated(int) ) , this , SLOT( changeDefaultWindowLevel( int ) ) );

    connect( m_stackedWidget , SIGNAL( currentChanged(int) ) , this , SLOT( pageChange(int) ) );

    connect( m_synchroCheckBox , SIGNAL( clicked(bool) ) , this , SLOT( synchronizeSlices(bool) ) );

    connect( m_chooseSeriePushButton , SIGNAL( clicked() ) , this , SLOT( chooseNewSerie() ) );

    // custom window level dialog
    connect( m_2DView , SIGNAL( windowLevelChanged( double , double ) ) , m_customWindowLevelDialog , SLOT( setDefaultWindowLevel( double , double ) ) );
    connect( m_2DView2_1 , SIGNAL( windowLevelChanged( double , double ) ) , m_customWindowLevelDialog , SLOT( setDefaultWindowLevel( double , double ) ) );
    connect( m_2DView2_2 , SIGNAL( windowLevelChanged( double , double ) ) , m_customWindowLevelDialog , SLOT( setDefaultWindowLevel( double , double ) ) );
}

void Q2DViewerExtension::setInput( Volume *input )
{
    m_mainVolume = input;
    // \TODO ara ho fem "a saco" per� s'hauria de millorar
    m_2DView->setInput( m_mainVolume );
    m_2DView2_1->setInput( m_mainVolume );
    m_2DView2_2->setInput( m_mainVolume );
    INFO_LOG("Q2DViewerExtension: Donem l'input principal")
    changeViewToAxial();
    
}

void Q2DViewerExtension::setSecondInput( Volume *input )
{
    m_secondaryVolume = input;
    // \TODO ara ho fem "a saco" per� s'hauria de millorar
    m_2DView2_2->setInput( m_secondaryVolume );
    INFO_LOG("Afegim un segon volum per comparar")
    changeViewToAxial();
    m_stackedWidget->setCurrentIndex( 1 );
}

void Q2DViewerExtension::populateToolBar( QToolBar *toolbar )
{
    if( toolbar )
    {
        toolbar->addAction( m_axialViewAction );
        toolbar->addAction( m_sagitalViewAction );
        toolbar->addAction( m_coronalViewAction );
    }
}

void Q2DViewerExtension::changeViewToAxial()
{
    m_currentView = Axial;
    switch( m_stackedWidget->currentIndex() )
    {
    case 0:
        m_spinBox->setMinimum( 0 );
        m_spinBox->setMaximum( m_mainVolume->getDimensions()[2] );
        m_slider->setMaximum(  m_mainVolume->getDimensions()[2] );
        m_slider->setValue( m_2DView->getSlice() );
        m_viewText->setText( tr("XY : Axial") );
        m_2DView->setViewToAxial();
        INFO_LOG("Visor per defecte: Canviem a vista axial (Vista 1)")
        m_2DView->render();
    break;

    case 1:
        int slices;
        if( m_secondaryVolume )
            slices = m_secondaryVolume->getDimensions()[2];
        else
            slices = m_mainVolume->getDimensions()[2];

        m_spinBox2_1->setMinimum( 0 );
        m_spinBox2_1->setMaximum( m_mainVolume->getDimensions()[2] );
        m_slider2_1->setMaximum(  m_mainVolume->getDimensions()[2] );
        m_slider2_1->setValue( m_2DView2_1->getSlice() );
        m_viewText2_1->setText( tr("XY : Axial") );
        m_2DView2_1->setViewToAxial();
        INFO_LOG("Visor per defecte: Canviem a vista axial (Vista 2.1)")
        m_2DView2_1->render();

        m_spinBox2_2->setMinimum( 0 );
        m_spinBox2_2->setMaximum( slices );
        m_slider2_2->setMaximum(  slices );
        m_slider2_2->setValue( m_2DView2_2->getSlice() );
        m_viewText2_2->setText( tr("XY : Axial") );
        m_2DView2_2->setViewToAxial();
        INFO_LOG("Visor per defecte: Canviem a vista axial (Vista 2.2)")
        m_2DView2_2->render();
    break;
    
    }

}

void Q2DViewerExtension::changeViewToSagital()
{
    m_currentView = Sagital;
    switch( m_stackedWidget->currentIndex() )
    {
    case 0:
        m_spinBox->setMinimum( 0 );
        m_spinBox->setMaximum( m_mainVolume->getDimensions()[0] );
        m_slider->setMaximum(  m_mainVolume->getDimensions()[0] );
        m_slider->setValue( m_mainVolume->getDimensions()[0]/2 );
        m_viewText->setText( tr("YZ : Sagital") );
        m_2DView->setViewToSagittal();
        INFO_LOG("Visor per defecte: Canviem a vista sagital (Vista 1)")
        m_2DView->render();
    break;

    case 1:
        int slices;
        if( m_secondaryVolume )
            slices = m_secondaryVolume->getDimensions()[0];
        else
            slices = m_mainVolume->getDimensions()[0];
            
        m_spinBox2_1->setMinimum( 0 );
        m_spinBox2_1->setMaximum( m_mainVolume->getDimensions()[0] );
        m_slider2_1->setMaximum(  m_mainVolume->getDimensions()[0] );
        m_slider2_1->setValue( m_2DView2_1->getSlice() );
        m_viewText2_1->setText( tr("YZ : Sagital") );
        m_2DView2_1->setViewToSagittal();
        INFO_LOG("Visor per defecte: Canviem a vista sagital (Vista 2.1)")
        m_2DView2_1->render();

        m_spinBox2_2->setMinimum( 0 );
        m_spinBox2_2->setMaximum( slices );
        m_slider2_2->setMaximum(  slices );
        m_slider2_2->setValue( m_2DView2_2->getSlice() );
        m_viewText2_2->setText( tr("YZ : Sagital") );
        m_2DView2_2->setViewToSagittal();
        INFO_LOG("Visor per defecte: Canviem a vista sagital (Vista 2.2)")
        m_2DView2_2->render();
    break;
    }
}

void Q2DViewerExtension::changeViewToCoronal()
{
    m_currentView = Coronal;
    switch( m_stackedWidget->currentIndex() )
    {
    case 0:
        m_spinBox->setMinimum( 0 );
        m_spinBox->setMaximum( m_mainVolume->getDimensions()[1] );
        m_slider->setMaximum(  m_mainVolume->getDimensions()[1] );
        m_slider->setValue( m_mainVolume->getDimensions()[1]/2 );
        m_viewText->setText( tr("XZ : Coronal") );
        m_2DView->setViewToCoronal();
        INFO_LOG("Visor per defecte: Canviem a vista coronal (Vista 1)")
        m_2DView->render();
    break;
    
    case 1:
        int slices;
        if( m_secondaryVolume )
            slices = m_secondaryVolume->getDimensions()[1];
        else
            slices = m_mainVolume->getDimensions()[1];
            
        m_spinBox2_1->setMinimum( 0 );
        m_spinBox2_1->setMaximum( m_mainVolume->getDimensions()[1] );
        m_slider2_1->setMaximum(  m_mainVolume->getDimensions()[1] );
        m_slider2_1->setValue( m_2DView2_1->getSlice() );
        m_viewText2_1->setText( tr("XZ : Coronal") );
        m_2DView2_1->setViewToCoronal();
        INFO_LOG("Visor per defecte: Canviem a vista coronal (Vista 2.1)")
        m_2DView2_1->render();

        m_spinBox2_2->setMinimum( 0 );
        m_spinBox2_2->setMaximum( slices );
        m_slider2_2->setMaximum(  slices );
        m_slider2_2->setValue( m_2DView2_2->getSlice() );
        m_viewText2_2->setText( tr("XZ : Coronal") );
        m_2DView2_2->setViewToCoronal();
        INFO_LOG("Visor per defecte: Canviem a vista coronal (Vista 2.2)")
        m_2DView2_2->render();
    break;
    }
}

void Q2DViewerExtension::changeDefaultWindowLevel( int which )
{
    // \TODO ara anem a saco per� 'shauria de fer una manera perqu� nom�s es cridessin els que cal
    switch( which )
    {
    case 0:
        m_2DView->resetWindowLevelToDefault();
        m_2DView2_1->resetWindowLevelToDefault();
        m_2DView2_2->resetWindowLevelToDefault();
        INFO_LOG("Visor per defecte: Canviem Window Level >> Defecte");
    break;

    case 1:
        m_2DView->resetWindowLevelToBone();
        m_2DView2_1->resetWindowLevelToBone();
        m_2DView2_2->resetWindowLevelToBone();
        INFO_LOG("Visor per defecte: Canviem Window Level >> Bone");
    break;

    case 2:
        m_2DView->resetWindowLevelToLung();
        m_2DView2_1->resetWindowLevelToLung();
        m_2DView2_2->resetWindowLevelToLung();
        INFO_LOG("Visor per defecte: Canviem Window Level >> Lung");
    break;

    case 3:
        m_2DView->resetWindowLevelToSoftTissuesNonContrast();
        m_2DView2_1->resetWindowLevelToSoftTissuesNonContrast();
        m_2DView2_2->resetWindowLevelToSoftTissuesNonContrast();
        INFO_LOG("Visor per defecte: Canviem Window Level >> SoftTissuesNC");
    break;

    case 4:
        m_2DView->resetWindowLevelToLiverNonContrast();
        m_2DView2_1->resetWindowLevelToLiverNonContrast();
        m_2DView2_2->resetWindowLevelToLiverNonContrast();
        INFO_LOG("Visor per defecte: Canviem Window Level >> LiverNC");
    break;

    case 5:
        m_2DView->resetWindowLevelToSoftTissuesContrastMedium();
        m_2DView2_1->resetWindowLevelToSoftTissuesContrastMedium();
        m_2DView2_2->resetWindowLevelToSoftTissuesContrastMedium();
        INFO_LOG("Visor per defecte: Canviem Window Level >> SoftTissuesCM");
    break;

    case 6:
        m_2DView->resetWindowLevelToLiverContrastMedium();
        m_2DView2_1->resetWindowLevelToLiverContrastMedium();
        m_2DView2_2->resetWindowLevelToLiverContrastMedium();
        INFO_LOG("Visor per defecte: Canviem Window Level >> LiverCM");
    break;

    case 7:
        m_2DView->resetWindowLevelToNeckContrastMedium();
        m_2DView2_1->resetWindowLevelToNeckContrastMedium();
        m_2DView2_2->resetWindowLevelToNeckContrastMedium();
        INFO_LOG("Visor per defecte: Canviem Window Level >> NeckCM");
    break;

    case 8:
        m_2DView->resetWindowLevelToAngiography();
        m_2DView2_1->resetWindowLevelToAngiography();
        m_2DView2_2->resetWindowLevelToAngiography();
        INFO_LOG("Visor per defecte: Canviem Window Level >> Angiography");
    break;

    case 9:
        m_2DView->resetWindowLevelToOsteoporosis();
        m_2DView2_1->resetWindowLevelToOsteoporosis();
        m_2DView2_2->resetWindowLevelToOsteoporosis();
        INFO_LOG("Visor per defecte: Canviem Window Level >> Osteoporosis");
    break;

    case 10:
        m_2DView->resetWindowLevelToEmphysema();
        m_2DView2_1->resetWindowLevelToEmphysema();
        m_2DView2_2->resetWindowLevelToEmphysema();
        INFO_LOG("Visor per defecte: Canviem Window Level >> Emfisema");
    break;
    
    case 11:
        m_2DView->resetWindowLevelToPetrousBone();
        m_2DView2_1->resetWindowLevelToPetrousBone();
        m_2DView2_2->resetWindowLevelToPetrousBone();
        INFO_LOG("Visor per defecte: Canviem Window Level >> Petrous Bone");
    break;

    case 12:
        // custom
        m_customWindowLevelDialog->exec();
        INFO_LOG("Visor per defecte: Canviem Window Level >> Custom");
    break;

    default:
        m_2DView->resetWindowLevelToDefault();
        m_2DView2_1->resetWindowLevelToDefault();
        m_2DView2_2->resetWindowLevelToDefault();
        INFO_LOG("Visor per defecte: Canviem Window Level >> Defecte");
    break;
    
    }
}

void Q2DViewerExtension::setView( ViewType view )
{
    switch( view )
    {
    case Axial:
        changeViewToAxial();
    break;
    case Sagital:
        changeViewToSagital();
    break;
    case Coronal:
        changeViewToCoronal();
    break;
    }
}

void Q2DViewerExtension::changeViewToSingle()
{
    m_stackedWidget->setCurrentIndex( 0 );
}

void Q2DViewerExtension::changeViewToDouble()
{
    m_stackedWidget->setCurrentIndex( 1 );
}

void Q2DViewerExtension::pageChange( int index )
{
    setView( m_currentView );
}

void Q2DViewerExtension::synchronizeSlices( bool ok )
{
    if( ok )
    {
        INFO_LOG("Visor per defecte: Sincronitzem llesques");
        connect( m_slider2_1 , SIGNAL( valueChanged(int) ) , m_slider2_2 , SLOT( setValue(int) ) );
        connect( m_slider2_2 , SIGNAL( valueChanged(int) ) , m_slider2_1 , SLOT( setValue(int) ) );
    }
    else
    {
        INFO_LOG("Visor per defecte: Desincronitzem llesques");
        disconnect( m_slider2_1 , SIGNAL( valueChanged(int) ) , m_slider2_2 , SLOT( setValue(int) ) );
        disconnect( m_slider2_2 , SIGNAL( valueChanged(int) ) , m_slider2_1 , SLOT( setValue(int) ) );
    }
}

void Q2DViewerExtension::chooseNewSerie()
{
    emit newSerie();
}

}
