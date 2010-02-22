/***************************************************************************
 *   Copyright (C) 2005-2006 by Grup de Gràfics de Girona                  *
 *   http://iiia.udg.es/GGG/index.html?langu=uk                            *
 *                                                                         *
 *   Universitat de Girona                                                 *
 ***************************************************************************/
#ifndef UDGDICOMTAG_CPP
#define UDGDICOMTAG_CPP

#include "dicomtag.h"

#include <QString>

namespace udg {

DICOMTag::DICOMTag()
{
}

DICOMTag::~DICOMTag()
{
}

DICOMTag::DICOMTag( unsigned int group, unsigned int element )
{
    m_group = group;
    m_element = element;
}

DICOMTag::DICOMTag( DICOMTag * tag )
{
    m_group = tag->getGroup();
    m_element = tag->getElement();
}

void DICOMTag::setGroup( unsigned int group )
{
    m_group = group;
}

void DICOMTag::setElement( unsigned int element )
{
    m_element = element;
}

unsigned int DICOMTag::getGroup()
{
    return m_group;
}

unsigned int DICOMTag::getElement()
{
    return m_element;
}

QString DICOMTag::toString()
{
    return QString("(%1,%2)").arg( QString::number(m_group,16).rightJustified(4,'0') ).arg( QString::number(m_element,16).rightJustified(4,'0') );
}

}

#endif
