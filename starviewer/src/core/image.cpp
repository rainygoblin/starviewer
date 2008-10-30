/***************************************************************************
 *   Copyright (C) 2005-2006 by Grup de Gràfics de Girona                  *
 *   http://iiia.udg.es/GGG/index.html?langu=uk                            *
 *                                                                         *
 *   Universitat de Girona                                                 *
 ***************************************************************************/
#include "image.h"

#include "logging.h"

#include <QStringList>
#include <QPainter>
#include <QBuffer>
#include <QPixmap>

#include <vtkMath.h> // pel ::Cross()

// fem servir dcmtk per l'escalat de les imatges dicom
#include "dcmtk/dcmimgle/dcmimage.h"
#include "dcmtk/ofstd/ofbmanip.h"

namespace udg {

Image::Image(QObject *parent)
 : QObject(parent), m_sliceThickness(0.0), m_samplesPerPixel(1), m_photometricInterpretation("MONOCHROME2"), m_rows(0), m_columns(0), m_bitsAllocated(16), m_bitsStored(16), m_pixelRepresentation(0), m_rescaleSlope(1), m_rescaleIntercept(0), m_numberOfFrames(1), m_parentSeries(NULL)
{
    m_pixelSpacing[0] = 0.;
    m_pixelSpacing[1] = 0.;
    memset( m_imageOrientationPatient, 0, 9*sizeof(double) );
    memset( m_imagePositionPatient, 0, 3*sizeof(double) );
}

Image::~Image()
{
}

void Image::setSOPInstanceUID( QString uid )
{
    m_SOPInstanceUID = uid;
}

QString Image::getSOPInstanceUID() const
{
    return m_SOPInstanceUID;
}

void Image::setInstanceNumber( QString number )
{
    m_instanceNumber = number;
}

QString Image::getInstanceNumber() const
{
    return m_instanceNumber;
}

void Image::setImageOrientationPatient( double orientation[6] )
{
    memcpy( m_imageOrientationPatient, orientation, 6*sizeof(double) );

    double normal[3];
    // calculem la Z
    vtkMath::Cross( &orientation[0] , &orientation[3] , normal );

    memcpy( &m_imageOrientationPatient[6], normal, 3*sizeof(double) );
}

void Image::setImageOrientationPatient( double xVector[3], double yVector[3] )
{
    memcpy( m_imageOrientationPatient, xVector, 3*sizeof(double) );
    memcpy( &m_imageOrientationPatient[3], yVector, 3*sizeof(double) );
    double normal[3];
    // calculem la Z
    vtkMath::Cross( xVector , yVector , normal );

    memcpy( &m_imageOrientationPatient[6], normal, 3*sizeof(double) );
}

const double* Image::getImageOrientationPatient() const
{
    return m_imageOrientationPatient;
}

void Image::setPatientOrientation( QString orientation )
{
    m_patientOrientation = orientation;
}

QString Image::getPatientOrientation() const
{
    return m_patientOrientation;
}

QString Image::getOrientationLabel()
{
    QString label;

    QStringList axisList = m_patientOrientation.split(",");

    QString rowAxis = axisList.at(0);
    QString columnAxis = axisList.at(1);

    if( !rowAxis.isEmpty() && !columnAxis.isEmpty() )
    {
        if( (rowAxis.startsWith("R") || rowAxis.startsWith("L")) && (columnAxis.startsWith("A") || columnAxis.startsWith("P")) )
            label="AXIAL";
        else if( (columnAxis.startsWith("R") || columnAxis.startsWith("L")) && (rowAxis.startsWith("A") || rowAxis.startsWith("P")) )
            label="AXIAL";
        else if ( (rowAxis.startsWith("R") || rowAxis.startsWith("L")) && (columnAxis.startsWith("S") || columnAxis.startsWith("I")) )
            label="CORONAL";
        else if( (columnAxis.startsWith("R") || columnAxis.startsWith("L")) && (rowAxis.startsWith("S") || rowAxis.startsWith("I")) )
            label="CORONAL";
        else if( (rowAxis.startsWith("A") || rowAxis.startsWith("P")) && (columnAxis.startsWith("S") || columnAxis.startsWith("I")) )
            label="SAGITAL";
        else if( (columnAxis.startsWith("A") || columnAxis.startsWith("P")) && (rowAxis.startsWith("S") || rowAxis.startsWith("I")) )
            label="SAGITAL";
    }
    else
    {
        label="OBLIQUE";
    }

    return label;
}

void Image::setPixelSpacing( double x, double y )
{
    m_pixelSpacing[0] = x;
    m_pixelSpacing[1] = y;
}

const double *Image::getPixelSpacing() const
{
    return m_pixelSpacing;
}

void Image::setSliceThickness( double z )
{
    m_sliceThickness = z;
}

double Image::getSliceThickness() const
{
    return m_sliceThickness;
}

void Image::setImagePositionPatient( double position[3] )
{
    memcpy( m_imagePositionPatient, position, 3*sizeof(double) );
}

const double *Image::getImagePositionPatient() const
{
    return m_imagePositionPatient;
}

void Image::getImagePlaneNormal( double normal[3] )
{
	normal[0] = m_imageOrientationPatient[6];
	normal[1] = m_imageOrientationPatient[7];
	normal[2] = m_imageOrientationPatient[8];
}

void Image::setSamplesPerPixel( int samples )
{
    m_samplesPerPixel = samples;
}

int Image::getSamplesPerPixel() const
{
    return m_samplesPerPixel;
}

void Image::setPhotometricInterpretation( QString value )
{
    m_photometricInterpretation = value;
}

QString Image::getPhotometricInterpretation() const
{
    return m_photometricInterpretation;
}

void Image::setRows( int rows  )
{
    m_rows = rows;
}

int Image::getRows() const
{
    return m_rows;
}

void Image::setColumns( int columns  )
{
    m_columns = columns;
}

int Image::getColumns() const
{
    return m_columns;
}

void Image::setBitsAllocated( int bits )
{
    m_bitsAllocated = bits;
}

int Image::getBitsAllocated() const
{
    return m_bitsAllocated;
}

void Image::setBitsStored( int bits )
{
    m_bitsStored = bits;
}

int Image::getBitsStored() const
{
    return m_bitsStored;
}

void Image::setPixelRepresentation( int representation )
{
    m_pixelRepresentation = representation;
}

int Image::getPixelRepresentation() const
{
    return m_pixelRepresentation;
}

void Image::setRescaleSlope( double slope )
{
    m_rescaleSlope = slope;
}

double Image::getRescaleSlope() const
{
    return m_rescaleSlope;
}

void Image::setRescaleIntercept( double intercept )
{
    m_rescaleIntercept = intercept;
}

double Image::getRescaleIntercept() const
{
    return m_rescaleIntercept;
}

void Image::setSliceLocation( QString sliceLocation )
{
    m_sliceLocation = sliceLocation;
}

QString Image::getSliceLocation() const
{
    return m_sliceLocation;
}

void Image::addWindowLevel( double window, double level )
{
    QPair<double, double> windowLevel( window, level );
    m_windowLevelList << windowLevel;
}

QPair<double,double> Image::getWindowLevel( int index ) const
{
    if( index >= 0 && index < m_windowLevelList.size() )
        return m_windowLevelList.at(index);
    else
    {
        DEBUG_LOG("Index out of range");
        return QPair<double,double>();
    }
}

int Image::getNumberOfWindowLevels()
{
    return m_windowLevelList.size();
}

void Image::setNumberOfFrames( int frames )
{
    m_numberOfFrames = frames;
}

int Image::getNumberOfFrames() const
{
    return m_numberOfFrames;
}

void Image::addWindowLevelExplanation( QString explanation )
{
    m_windowLevelExplanationList << explanation;
}

void Image::setWindowLevelExplanations( const QStringList &explanations )
{
    m_windowLevelExplanationList = explanations;
}

QString Image::getWindowLevelExplanation( int index ) const
{
    if( index >= 0 && index < m_windowLevelExplanationList.size() )
        return m_windowLevelExplanationList.at(index);
    else
    {
        DEBUG_LOG("Index out of range");
        return QString();
    }
}

void Image::setRetrievedDate(QDate retrievedDate)
{
    m_retrievedDate = retrievedDate;
}

void Image::setRetrievedTime(QTime retrievedTime)
{
    m_retrieveTime = retrievedTime;
}

QDate Image::getRetrievedDate()
{
    return m_retrievedDate;
}

QTime Image::getRetrievedTime()
{
    return m_retrieveTime;
}

void Image::setParentSeries( Series *series )
{
    m_parentSeries = series;
}

Series *Image::getParentSeries() const
{
    return m_parentSeries;
}

void Image::setPath( QString path )
{
    m_path = path;
}

QString Image::getPath() const
{
    return m_path;
}

void Image::addReferencedImage( Image *image )
{
    QString uid = image->getSOPInstanceUID();
    if( uid.isEmpty() )
    {
        DEBUG_LOG("L'uid de la imatge està buit! No la podem insertar per inconsistent");
    }
    else
    {
        m_referencedImageSequence.push_back( image );
    }

}

Image *Image::getReferencedImage( QString SOPInstanceUID ) const
{
    int i = 0;
    bool found = false;
    while( i < m_referencedImageSequence.size() && !found )
    {
        if( m_referencedImageSequence.at(i)->getSOPInstanceUID() == SOPInstanceUID )
            found = true;
        else
            i++;
    }
    if( !found )
        return 0;
    else
        return m_referencedImageSequence.at(i);
}

QList<Image *> Image::getReferencedImages() const
{
    return m_referencedImageSequence;
}

bool Image::hasReferencedImages() const
{
    return ! m_referencedImageSequence.isEmpty();
}

QPixmap Image::getThumbnail(int resolution)
{
    return QPixmap::fromImage( createThumbnail(resolution) );
}

QImage Image::createThumbnail(int resolution)
{
    if(! m_thumbnail.isNull() )
    {
        return m_thumbnail;
    }

    QImage thumbnail;
    bool ok = false;

    //carreguem el fitxer dicom a escalar
    DicomImage *dicomImage = new DicomImage( qPrintable( getPath() ) );

    if( dicomImage == NULL )
    {
        ok = false;
        DEBUG_LOG("Memòria insuficient per carregar l'imatge DICOM al fer el thumbnail");
    }
    else if( dicomImage->getStatus() == EIS_Normal )
    {
        dicomImage->hideAllOverlays();
        dicomImage->setMinMaxWindow(1);
        //escalem l'imatge
        DicomImage *scaledImage;
        //Escalem pel cantó més gran
        unsigned long width, height;
        if( dicomImage->getWidth() < dicomImage->getHeight() )
        {
            width = 0;
            height = resolution;
        }
        else
        {
            width = resolution;
            height = 0;
        }
        scaledImage = dicomImage->createScaledImage( width,height, 1, 1 );
        if( scaledImage == NULL )
        {
            ok = false;
            DEBUG_LOG("La imatge escalada s'ha retornat com a nul");
        }
        else if( scaledImage->getStatus() == EIS_Normal )
        {
            // el següent codi crea una imatge pgm a memòria i carreguem aquest buffer directament al pixmap
            // obtingut de http://forum.dcmtk.org/viewtopic.php?t=120&highlight=qpixmap
            // get image extension
            const int width = (int)(scaledImage->getWidth());
            const int height = (int)(scaledImage->getHeight());
            char header[32];
            // create PGM header
            sprintf(header, "P5\n%i %i\n255\n", width, height);
            const int offset = strlen(header);
            const unsigned int length = width * height + offset;
            // create output buffer for DicomImage class
            Uint8 *buffer = new Uint8[length];
            if (buffer != NULL)
            {
                // copy PGM header to buffer
                OFBitmanipTemplate<Uint8>::copyMem((const Uint8 *)header, buffer, offset);
                if( scaledImage->getOutputData((void *)(buffer + offset), length, 8))
                {
                    if( thumbnail.loadFromData((const unsigned char *)buffer, length, "PGM") )
                    {
                        ok = true;
                    }
                    else
                        DEBUG_LOG(">>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>Ha fallat :(");

                }
                // delete temporary pixel buffer
                delete[] buffer;
            }
        }
        else
        {
            ok = false;
            DEBUG_LOG( QString( "La imatge escalada té errors. Error: %1 ").arg( DicomImage::getString( scaledImage->getStatus() ) ) );
        }
    }
    else
    {
        ok = false;
        DEBUG_LOG( QString( "Error en carregar la DicomImage. Error: %1 ").arg( DicomImage::getString( dicomImage->getStatus() ) ) );
    }

    if( !ok ) // no hem pogut generar el thumbnail, creem un de buit
    {
        thumbnail = QImage(resolution, resolution, QImage::Format_RGB32);
        thumbnail.fill(Qt::black);

        QPainter painter( &thumbnail );
        painter.setPen(Qt::white);
        painter.drawText(0, 0, resolution, resolution, Qt::AlignCenter | Qt::TextWordWrap, tr("Preview image not available"));
    }
    m_thumbnail = thumbnail;
    return thumbnail;
}

}
