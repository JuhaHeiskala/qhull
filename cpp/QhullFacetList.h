/****************************************************************************
**
** Copyright (C) 2008-2009 C. Bradford Barber. All rights reserved.
** $Id: //product/qhull/main/rel/cpp/QhullFacetList.h#17 $$Change: 1102 $
** $DateTime: 2009/12/07 20:26:04 $$Author: bbarber $
**
****************************************************************************/

#ifndef QHULLFACETLIST_H
#define QHULLFACETLIST_H

#include "QhullLinkedList.h"

#include <ostream>

namespace orgQhull {

#//ClassRef
    class               QhullFacet;

#//Types
    //! QhullFacetList -- List of Qhull facets, as a C++ class.  See QhullFacetSet.h
    class               QhullFacetList;
    //! QhullFacetListIterator -- if(f.isGood()){ ... }
    typedef QhullLinkedListIterator<QhullFacet>  
                        QhullFacetListIterator;

class QhullFacetList : public QhullLinkedList<QhullFacet> {

#// Fields
private:
    bool                select_all;   //! True if include bad facets.  Default is false.

#//Constructors
public:
                        QhullFacetList(QhullFacet b, QhullFacet e) : QhullLinkedList<QhullFacet>(b, e), select_all(false) {}
                        //Copy constructor copies pointer but not contents.  Needed for return by value.
                        QhullFacetList(const QhullFacetList &o) : QhullLinkedList<QhullFacet>(*o.begin(), *o.end()), select_all(o.select_all) {}
                       ~QhullFacetList() {}

private:
                        //!Disable default constructor and copy assignment.  See QhullSetBase
                        QhullFacetList();
    QhullFacetList     &operator=(const QhullFacetList &);
public:

#//Conversion
#ifndef QHULL_NO_STL
    std::vector<QhullFacet> toStdVector() const;
    std::vector<QhullVertex> vertices_toStdVector(int qhRunId) const;
#endif //QHULL_NO_STL
#ifdef QHULL_USES_QT
    QList<QhullFacet>   toQList() const;
    QList<QhullVertex>  vertices_toQList(int qhRunId) const;
#endif //QHULL_USES_QT

#//GetSet
    bool                isSelectAll() const { return select_all; }
    void                selectAll() { select_all= true; }
    void                selectGood() { select_all= false; }

#//Read-only
                        //! Filtered by facet.isGood().  May be 0 when !isEmpty().
    int                 count() const;
    bool                contains(const QhullFacet &f) const;
    int                 count(const QhullFacet &f) const;
                        //! operator==() does not depend on isGood()

#//IO
    struct PrintFacetList{
        const QhullFacetList *facet_list;
        int             run_id;
                        PrintFacetList(int qhRunId, const QhullFacetList &fl) : facet_list(&fl), run_id(qhRunId) {} 
    };//PrintFacetList
    PrintFacetList     print(int qhRunId) const  { return PrintFacetList(qhRunId, *this); }

    struct PrintFacets{
        const QhullFacetList *facet_list;
        int             run_id;
                        PrintFacets(int qhRunId, const QhullFacetList &fl) : facet_list(&fl), run_id(qhRunId) {} 
    };//PrintFacets
    PrintFacets     printFacets(int qhRunId) const { return PrintFacets(qhRunId, *this); }

    struct PrintVertices{
        const QhullFacetList *facet_list; 
        int             run_id;   //! Can not be NOrunId due to qh_facetvertices
                        PrintVertices(int qhRunId, const QhullFacetList &fl) : facet_list(&fl), run_id(qhRunId) {} 
    };//PrintVertices
    PrintVertices   printVertices(int qhRunId) const { return PrintVertices(qhRunId, *this); }
};//class QhullFacetList

}//namespace orgQhull

#//== Global namespace =========================================

std::ostream &operator<<(std::ostream &os, const orgQhull::QhullFacetList::PrintFacetList &p);
std::ostream &operator<<(std::ostream &os, const orgQhull::QhullFacetList::PrintFacets &p);
std::ostream &operator<<(std::ostream &os, const orgQhull::QhullFacetList::PrintVertices &p);

// FIXUP -- why did <<facetList work and the others did not?
// print() not available since printVertices() requires qhRunId.
// [9/09] added const
inline std::ostream &operator<<(std::ostream &os, const orgQhull::QhullFacetList &fs) { os<< fs.printFacets(orgQhull::UsingLibQhull::NOqhRunId); return os; } 

#endif // QHULLFACETLIST_H
