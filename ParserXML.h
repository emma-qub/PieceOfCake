#ifndef PARSERXML_H
#define PARSERXML_H

#include <QtXml>

#include "Point2d.h"
#include "Segment.h"
#include "Polygon.h"
#include "Hint.h"

typedef QList<Polygon> PolygonList;
typedef QList<Hint> HintList;

class ParserXML {

public:
    ParserXML(void);
    ParserXML(QString xmlFileName);
    ParserXML(const ParserXML& parserXML);
    virtual ~ParserXML(void);

    inline int getPolygonNodesCount(void) const { return _polygonNodesCount; }
//    inline void setFileName(const QString& xmlFileName) { _xmlFileName = xmlFileName; }

    QDomElement intToNode(int value, const QString& nodeName);
    QDomElement pointToNode(const Point2d& point, const QString& nodeName);
    QDomElement segmentToNode(const Segment& segment);
    QDomElement partscountToNode(unsigned int partscount = 1);
    QDomElement linescountToNode(unsigned int linescount = 0);
    QDomElement starscountToNode(unsigned int starscount = 0);
    QDomElement tolerancesToNode(unsigned int tolerances = 1);
    QDomElement polygonToNode(const Polygon& polygon, int id);
    QDomElement hintToNode(const Hint& hint, int id);

    void addPolygon(const Polygon& polygon);
    void addHint(const Hint& hint);

    QDomElement getPolygon(int id);
    QDomElement getHint(int id);

    void setPartsCount(int partscount);
    void setLinesCount(int linescount);
    void setStarsCount(int starscount);
    void setTolerances(int tolerances);
    void replacePolygon(const Polygon& polygon, int id);
    void replaceHint(const Hint& hint, int id);

    bool removeElement(QDomElement& supElement, QDomElement& subElement, int id);
    bool removePolygon(int id);
    bool removeHint(int id);

    int createInt(const QDomElement& element, const QString& attributeName);
    float createFloat(const QDomElement& element, const QString& attributeName);
    Point2d createPoint(const QDomElement& element);
    Segment createSegment(const QDomElement& element);
    int getIntValue(const QString& tagName, const QString& attributeName = "value");
    int getPartsCount(void);
    int getLinesCount(void);
    int getStarsCount(void);
    int getTolerances(void);
    Polygon createPolygon(const QDomElement& element);
    Hint createHint(const QDomElement& element);

    PolygonList createPolygonList(void);
    HintList createHintList(void);

    inline QDomDocument getDoc() const { return _doc; }
    inline void initFileName(const QString& fileName) { if (_xmlFileName.isEmpty()) _xmlFileName = fileName; }

    ParserXML& operator=(const ParserXML& parser);

    void writeXML(int indent = 4);

private:
    QString _xmlFileName;
    QDomDocument _doc;
    QDomElement _polygons;
    QDomElement _hints;
    int _polygonNodesCount;
    int _hintNodesCount;
};

#endif // PARSERXML_H
