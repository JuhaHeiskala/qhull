/****************************************************************************
**
** Copyright (C) 2008-2009 C. Bradford Barber. All rights reserved.
** $Id: //product/qhull/main/rel/cpp/qhulltest/QhullFacetList_test.cpp#11 $$Change: 1102 $
** $DateTime: 2009/12/07 20:26:04 $$Author: bbarber $
**
****************************************************************************/

#include <iostream>
#include "../road/RoadTest.h" // QT_VERSION

#include "Qhull.h"
#include "QhullError.h"
#include "QhullFacet.h"
#include "QhullFacetList.h"
#include "QhullVertexSet.h"

using std::cout;
using std::endl;
using std::ostringstream;
using std::ostream;
using std::string;

namespace orgQhull {

class QhullFacetList_test : public RoadTest
{
    Q_OBJECT

#//Test slots
private slots:
    void cleanup();
    void t_construct();
    void t_convert();
    void t_readonly();
    void t_foreach();
    void t_io();
};//QhullFacetList_test

void
add_QhullFacetList_test()
{
    new QhullFacetList_test();
}

//Executed after each testcase
void QhullFacetList_test::
cleanup()
{
    UsingLibQhull::checkQhullMemoryEmpty();
    RoadTest::cleanup();
}

void QhullFacetList_test::
t_construct()
{ 
    RboxPoints rcube("c");
    Qhull q(rcube,"QR0");  // rotated unit cube
    QhullFacetList fs2= q.facetList();
    QVERIFY(!fs2.isEmpty());
    QCOMPARE(fs2.count(),6);
    QhullFacetList fs3(q.endFacet(), q.endFacet());
    QVERIFY(fs3.isEmpty());
    QhullFacetList fs4(q.endFacet().previous(), q.endFacet());
    QCOMPARE(fs4.count(), 1);
    QhullFacetList fs5(q.beginFacet(), q.endFacet());
    QCOMPARE(fs2.count(), fs5.count());
    QVERIFY(fs2==fs5);
    QhullFacetList fs6= fs2; // copy constructor
    QVERIFY(fs6==fs2);
    std::vector<QhullFacet> fv= fs2.toStdVector();
    QCOMPARE(fv.size(), 6u);
}//t_construct

void QhullFacetList_test::
t_convert()
{ 
    RboxPoints rcube("c");
    Qhull q(rcube,"QR0 QV2");  // rotated unit cube
    QhullFacetList fs2= q.facetList();
    QVERIFY(!fs2.isSelectAll());
    QVERIFY(!fs2.isEmpty());
    QCOMPARE(fs2.count(),3);
    std::vector<QhullFacet> fv= fs2.toStdVector();
    QCOMPARE(fv.size(), 3u);
    QList<QhullFacet> fv2= fs2.toQList();
    QCOMPARE(fv2.size(), 3);
    std::vector<QhullVertex> fv5= fs2.vertices_toStdVector(q.runId());
    QCOMPARE(fv5.size(), 7u);
    QList<QhullVertex> fv6= fs2.vertices_toQList(q.runId());
    QCOMPARE(fv6.size(), 7);
    fs2.selectAll();
    QVERIFY(fs2.isSelectAll());
    std::vector<QhullFacet> fv3= fs2.toStdVector();
    QCOMPARE(fv3.size(), 6u);
    QList<QhullFacet> fv4= fs2.toQList();
    QCOMPARE(fv4.size(), 6);
    std::vector<QhullVertex> fv7= fs2.vertices_toStdVector(q.runId());
    QCOMPARE(fv7.size(), 8u);
    QList<QhullVertex> fv8= fs2.vertices_toQList(q.runId());
    QCOMPARE(fv8.size(), 8);
}//t_convert

//! Spot check properties and read-only.  See QhullLinkedList_test
void QhullFacetList_test::
t_readonly()
{ 
    RboxPoints rcube("c");
    Qhull q(rcube,"QV0");  // good facets are adjacent to point 0
    QhullFacetList fs= q.facetList();
    QVERIFY(!fs.isSelectAll());
    QCOMPARE(fs.count(), 3);
    QCOMPARE(fs.first(), q.firstFacet());
    fs.selectAll();
    QVERIFY(fs.isSelectAll());
    QCOMPARE(fs.count(), 6);
    fs.selectGood();
    QVERIFY(!fs.isSelectAll());
    QCOMPARE(fs.count(), 3);
    fs.selectAll();
    QVERIFY(fs.isSelectAll());
    QCOMPARE(fs.count(), 6);
    QhullFacet f= fs.first();
    QhullFacet f2= fs.last();
    fs.selectAll();
    QVERIFY(fs.contains(f));
    QVERIFY(fs.contains(f2));
    QVERIFY(f.isGood());
    QVERIFY(!f2.isGood());
    fs.selectGood();
    QVERIFY(fs.contains(f));
    QVERIFY(!fs.contains(f2));
}//t_readonly

void QhullFacetList_test::
t_foreach()
{   
    RboxPoints rcube("c");
    // Spot check predicates and accessors.  See QhullLinkedList_test
    Qhull q(rcube,"Qt QR0");  // triangulation of rotated unit cube
    QhullFacetList fs= q.facetList();
    QVERIFY(fs.contains(q.firstFacet()));
    QhullFacet f= q.firstFacet().next();
    QVERIFY(fs.contains(f));
    QCOMPARE(fs.first(), *fs.begin());
    QCOMPARE(*(fs.end()-1), fs.last());
    QCOMPARE(fs.first(), q.firstFacet());
    QCOMPARE(*fs.begin(), q.beginFacet());
    QCOMPARE(*fs.end(), q.endFacet());
}//t_foreach

void QhullFacetList_test::
t_io()
{   
    RboxPoints rcube("c");
    {
        Qhull q(rcube,"QR0 QV0");   // good facets are adjacent to point 0
        QhullFacetList fs= q.facetList();
        ostringstream os;
        os << fs.print(q.runId()); // Runs all print options
        os << "\nFacets only\n" << fs; // printVertices() requires a runId
        cout<< os.str();
        QString facets= QString::fromStdString(os.str());
        QCOMPARE(facets.count("(v"), 7+12*3*2);
        QCOMPARE(facets.count(QRegExp("f\\d")), 3*7 + 13*3*2);
    }
}//t_io

//FIXUP -- Move conditional, QhullFacetSet code to QhullFacetSet.cpp
#ifndef QHULL_NO_STL
std::vector<QhullFacet> QhullFacetList:: 
toStdVector() const
{
    QhullLinkedListIterator<QhullFacet> i(*this);
    std::vector<QhullFacet> vs;
    while(i.hasNext()){
        QhullFacet f= i.next();
        if(isSelectAll() || f.isGood()){
            vs.push_back(f);
        }
    }
    return vs;
}//toStdVector
#endif //QHULL_NO_STL

#ifdef QHULL_USES_QT
QList<QhullFacet> QhullFacetList::
toQList() const
{
    QhullLinkedListIterator<QhullFacet> i(*this);
    QList<QhullFacet> vs;
    while(i.hasNext()){
        QhullFacet f= i.next();
        if(isSelectAll() || f.isGood()){
            vs.append(f);
        }
    }
    return vs;
}//toQList
#endif //QHULL_USES_QT

#ifndef QHULL_NO_STL
//! Same as PrintVertices
std::vector<QhullVertex> QhullFacetList:: 
vertices_toStdVector(int qhRunId) const
{
    std::vector<QhullVertex> vs;
    QhullVertexSet qvs(qhRunId, first().getFacetT(), NULL, isSelectAll());

    for(QhullVertexSet::iterator i=qvs.begin(); i!=qvs.end(); ++i){
        vs.push_back(*i);
    }
    return vs;
}//vertices_toStdVector
#endif //QHULL_NO_STL

#ifdef QHULL_USES_QT
//! Same as PrintVertices
QList<QhullVertex> QhullFacetList::
vertices_toQList(int qhRunId) const
{
    QList<QhullVertex> vs;
    QhullVertexSet qvs(qhRunId, first().getFacetT(), NULL, isSelectAll());
    for(QhullVertexSet::iterator i=qvs.begin(); i!=qvs.end(); ++i){
        vs.push_back(*i);
    }
    return vs;
}//vertices_toQList
#endif //QHULL_USES_QT
}//orgQhull

#include "moc/QhullFacetList_test.moc"
