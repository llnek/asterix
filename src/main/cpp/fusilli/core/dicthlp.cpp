static const char* const rcsid = "@(#)TIBCO Object Framework (TOF) $Id: tdicthlp.cpp,v 1.2 1998/08/12 05:52:57 kenl Exp $";

// Embedded release string for release builds
// Builds with -DRELEASE_VERSION="\"<PROJECT> <MAJOR_RELEASE> <PATCH_RELEASE>\""
// defined will embed the release string in the binary

#ifdef RELEASE_VERSION
static const char* const releaseId = "@(#)TIBCO Object Framework (TOF) TOFCore Release: " RELEASE_VERSION;
#else
static const char* const releaseId = "@(#)TIBCO Object Framework (TOF) TOFCore Release: Internal Build";
#endif

//===========================================================================
//
// $RCSfile: tdicthlp.cpp,v $ $Revision: 1.2 $ $Date: 1998/08/12 05:52:57 $
//
// Project:	TIBCO Object Framework (TOF)
//
// Description:	Implementation for class Tassoc, Tpool
//
// Author:	Ken Leung
//
// Copyright (c) 1997 TIBCO Software, Inc.
//___________________________________________________________________________

//---------------------- Includes and Definitions ---------------------------

#include <malloc.h>
#include "tdicthlp.h"

//------------------------------ Externals ----------------------------------


//--------------------------------- Local -----------------------------------


//----------------------------- Module Code ---------------------------------

//===========================================================================
// Name:
//
// Description:
//
//___________________________________________________________________________

void Tpool::FreeDataChain( Tpool*& head)
{
    Tpool*	pn;
    Tpool*	p = head;

    while ( p != NULL)
    {
	pn = p->m_next;
	mc_free_ptr(p->m_data);
	mc_free_ptr(p);
	p = pn;
    }

    head = NULL;
}

//===========================================================================
// Name:
//
// Description:
//
//___________________________________________________________________________

Tpool* Tpool::Create(Tpool*& head, UINT nMax)
{
    Tpool* 	p = mc_pcast(Tpool, malloc(sizeof(Tpool)));
    p->m_data 	= calloc(nMax, sizeof(Tassoc));
    p->m_next 	= head;
    head	= p;
    return p;
}

//------------------------- End Of File --------------------------------------
