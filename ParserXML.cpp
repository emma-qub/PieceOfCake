#include "ParserXML.h"

#include <QDebug>

/****************
 * Constructors *
 ****************/
ParserXML::ParserXML(void):
  _xmlFileName(),
  _doc("PieceOfCakeML"),
  _polygonNodesCount(0),
  _tapeNodesCount(0),
  _mirrorNodesCount(0),
  _hintNodesCount(0) {

  QDomElement root = _doc.createElement("level");
  _doc.appendChild(root);

  _polygons = _doc.createElement("polygons");
  root.appendChild(_polygons);

  _lineModifiers = _doc.createElement("linemodifiers");
  root.appendChild(_lineModifiers);

  _tapes = _doc.createElement("tapes");
  _lineModifiers.appendChild(_tapes);

  _mirrors = _doc.createElement("mirrors");
  _lineModifiers.appendChild(_mirrors);

  root.appendChild(_doc.createElement("linescount"));

  root.appendChild(_doc.createElement("partscount"));

  root.appendChild(_doc.createElement("maxgaptowin"));

  root.appendChild(_doc.createElement("starscount"));

  _hints = _doc.createElement("hints");
  root.appendChild(_hints);
}

ParserXML::ParserXML(const QString& xmlFileName):
  _xmlFileName(xmlFileName),
  _doc(QDomDocument("PieceOfCakeML")) {

  QFile XMLDoc(_xmlFileName);
  if (!XMLDoc.exists()) {
    qDebug() << "Error:" << _xmlFileName << "file not found";
    return;
  }

  if(!XMLDoc.open(QIODevice::ReadOnly | QIODevice::Text)) {
    qDebug() << "Cannot open XML file in ParserXML::ParserXML(QString xmlFileName)";
    return;
  }

  if(!_doc.setContent(&XMLDoc)) {
    XMLDoc.close();
    qDebug() << "Cannot set content of dom in ParserXML::ParserXML(QString xmlFileName)";
    return;
  }

  _polygons = _doc.firstChildElement("level").firstChildElement("polygons");
  _tapes = _doc.firstChildElement("level").firstChildElement("linemodifiers").firstChildElement("tapes");
  _mirrors = _doc.firstChildElement("level").firstChildElement("linemodifiers").firstChildElement("mirrors");
  _hints = _doc.firstChildElement("level").firstChildElement("hints");

  _polygonNodesCount = _polygons.elementsByTagName("polygon").count();
  _tapeNodesCount = _tapes.elementsByTagName("tape").count();
  _mirrorNodesCount = _mirrors.elementsByTagName("mirror").count();
  _hintNodesCount = _hints.elementsByTagName("hint").count();

  XMLDoc.close();
}

ParserXML::ParserXML(const ParserXML& parserXML) {
  _xmlFileName = parserXML._xmlFileName;
  _doc = parserXML._doc;
}

/***************
 * Destructors *
 ***************/
ParserXML::~ParserXML() {
}

/***********
 * Methods *
 ***********/

/********************************
 * Transform objects into nodes *
 ********************************/
QDomElement ParserXML::intToNode(int value, const QString& nodeName) {
  QDomElement element(_doc.createElement(nodeName));
  element.setAttribute("value", value);
  return element;
}

QDomElement ParserXML::pointToNode(const Point2d& point, const QString& nodeName) {
  QDomElement element(_doc.createElement(nodeName));
  element.setAttribute("x", point.getX());
  element.setAttribute("y", point.getY());
  return element;
}

QDomElement ParserXML::segmentToNode(const Segment& segment) {
  QDomElement element(_doc.createElement("Line"));
  element.appendChild(pointToNode(segment.getA(), "bound"));
  element.appendChild(pointToNode(segment.getB(), "bound"));
  return element;
}

QDomElement ParserXML::partscountToNode(unsigned int partscount) {
  return intToNode(partscount, "partscount");
}

QDomElement ParserXML::linescountToNode(unsigned int linescount) {
  return intToNode(linescount, "linescount");
}

QDomElement ParserXML::starscountToNode(unsigned int starscount) {
  return intToNode(starscount, "starscount");
}

QDomElement ParserXML::tolerancesToNode(unsigned int tolerances) {
  return intToNode(tolerances, "tolerances");
}

QDomElement ParserXML::polygonToNode(const Polygon& polygon, int id) {
  QDomElement element(_doc.createElement("polygon"));
  element.setAttribute("id", id);

  for (const Point2d& vertex: polygon.getVertices()) {
    element.appendChild(pointToNode(vertex, "vertex"));
  }

  return element;
}

QDomElement ParserXML::tapeToNode(const Tape& tape, int id) {
  QDomElement element(_doc.createElement("tape"));
  element.setAttribute("id", id);

  element.setAttribute("x", tape.getX());
  element.setAttribute("y", tape.getY());
  element.setAttribute("w", tape.getW());
  element.setAttribute("h", tape.getH());

  return element;
}

QDomElement ParserXML::mirrorToNode(const Mirror& mirror, int id) {
  QDomElement element(_doc.createElement("mirror"));
  element.setAttribute("id", id);

  Segment mirrorLine(mirror.getMirrorLine());
  element.setAttribute("xa", mirrorLine.getA().getX());
  element.setAttribute("ya", mirrorLine.getA().getY());
  element.setAttribute("xb", mirrorLine.getB().getX());
  element.setAttribute("yb", mirrorLine.getB().getY());

  return element;
}

QDomElement ParserXML::hintToNode(const Hint& hint, int id) {
  QDomElement element(_doc.createElement("hint"));
  element.setAttribute("id", id);

  element.appendChild(segmentToNode(hint.getLine()));
  element.appendChild(intToNode(hint.getCost(), "cost"));

  return element;
}

void ParserXML::addPolygon(const Polygon& polygon) {
  _polygons.appendChild(polygonToNode(polygon, _polygonNodesCount++));
}

void ParserXML::addTape(const Tape& tape) {
  _tapes.appendChild(tapeToNode(tape, _tapeNodesCount++));
}

void ParserXML::addMirror(const Mirror& mirror) {
  _mirrors.appendChild(mirrorToNode(mirror, _mirrorNodesCount++));
}

void ParserXML::addHint(const Hint& hint) {
  _hints.appendChild(hintToNode(hint, _hintNodesCount++));
}

QDomElement ParserXML::getElementById(const QDomElement& parent, const QString& name, int id)  {
  QDomNodeList nodeList = parent.elementsByTagName(name);
  for (int k = 0; k < nodeList.size(); k++) {
    QDomElement element = nodeList.at(k).toElement();
    if (element.attribute("id").toInt() == id) {
      return element;
    }
  }


  qDebug() << "Error: cannot find element " << name << " with number" << QString::number(id);
  return QDomElement();
}

QDomElement ParserXML::getPolygon(int id) {
  return getElementById(_polygons, "polygon", id);
}

QDomElement ParserXML::getTape(int id) {
  return getElementById(_tapes, "tape", id);
}

QDomElement ParserXML::getMirror(int id) {
  return getElementById(_mirrors, "mirror", id);
}

QDomElement ParserXML::getHint(int id) {
  return getElementById(_hints, "hint", id);
}

void ParserXML::setPartsCount(int partscount) {
  _doc.elementsByTagName("partscount").at(0).toElement().setAttribute("value", partscount);
}

void ParserXML::setLinesCount(int linescount) {
  _doc.elementsByTagName("linescount").at(0).toElement().setAttribute("value", linescount);
}

void ParserXML::setMaxGapToWin(int maxgaptowin) {
  _doc.elementsByTagName("maxgaptowin").at(0).toElement().setAttribute("value", maxgaptowin);
}

void ParserXML::setStarsCount(int starscount) {
  _doc.elementsByTagName("starscount").at(0).toElement().setAttribute("value", starscount);
}

void ParserXML::setTolerances(int tolerances) {
  _doc.elementsByTagName("tolerances").at(0).toElement().setAttribute("value", tolerances);
}

void ParserXML::replacePolygon(const Polygon& polygon, int id) {
  _polygons.replaceChild(polygonToNode(polygon, id), getPolygon(id));
}

void ParserXML::replaceTape(const Tape& tape, int id) {
  _tapes.replaceChild(tapeToNode(tape, id), getTape(id));
}

void ParserXML::replaceMirror(const Mirror& mirror, int id) {
  _tapes.replaceChild(mirrorToNode(mirror, id), getMirror(id));
}

void ParserXML::replaceHint(const Hint& hint, int id) {
  _hints.replaceChild(hintToNode(hint, id), getHint(id));
}

bool ParserXML::removeElement(QDomElement& supElement, QDomElement& subElement, int id) {
  QString tagName = subElement.tagName();

  if (subElement.isNull()) {
    qDebug() << "Cannot remove" << tagName << id << "whithin ParserXML::removeElement: wrong id.";
    return false;
  }

  if ((supElement.removeChild(subElement).isNull())) {
    qDebug() << "Cannot remove" << tagName << id << "whithin ParserXML::removePolygon: removeChild error.";
    return false;
  }

  QDomNodeList nodeList = supElement.elementsByTagName(tagName);
  for (int k = 0; k < nodeList.size(); k++) {
    QDomElement element = nodeList.at(k).toElement();
    int currId = element.attribute("id").toInt();
    if (currId > id) {
      element.setAttribute("id", currId-1);
    }
  }

  return true;
}

bool ParserXML::removePolygon(int id) {
  QDomElement element(getPolygon(id));
  return removeElement(_polygons, element, id);
}

bool ParserXML::removeTape(int id) {
  QDomElement element(getTape(id));
  return removeElement(_tapes, element, id);
}

bool ParserXML::removeMirror(int id) {
  QDomElement element(getMirror(id));
  return removeElement(_mirrors, element, id);
}

bool ParserXML::removeHint(int id) {
  QDomElement element(getHint(id));
  return removeElement(_hints, element, id);
}

int ParserXML::createInt(const QDomElement& element, const QString& attributeName) {
  return element.attribute(attributeName).toInt();
}

float ParserXML::createFloat(const QDomElement& element, const QString& attributeName) {
  return element.attribute(attributeName).toFloat();
}

Point2d ParserXML::createPoint(const QDomElement& element) {
  return Point2d(createFloat(element, "x"), createFloat(element, "y"));
}

Segment ParserXML::createSegment(const QDomElement& element) {
  QDomNodeList bounds = element.elementsByTagName("bound");
  if (bounds.size() != 2) {
    qDebug() << "Cannot create segment whithin ParserXML::createSegment: file corrupted";
    return Segment();
  }

  return Segment(createPoint(bounds.at(0).toElement()), createPoint(bounds.at(1).toElement()));
}

int ParserXML::getIntValue(const QString& tagName, const QString& attributeName) {
  QDomNodeList nodeList = _doc.elementsByTagName(tagName);
  if (nodeList.size() != 1)
    return -1;

  const QDomElement& element = nodeList.at(0).toElement();
  return createInt(element, attributeName);
}

int ParserXML::getPartsCount(void) {
  return getIntValue("partscount");
}

int ParserXML::getLinesCount(void) {
  return getIntValue("linescount");
}

int ParserXML::getMaxGapToWin(void) {
  return getIntValue("maxgaptowin");
}

int ParserXML::getStarsCount(void) {
  return getIntValue("starscount");
}

int ParserXML::getTolerances(void) {
  return getIntValue("tolerances");
}

Polygon ParserXML::createPolygon(const QDomElement& element) {
  Polygon polygon;

  QDomNodeList vertices = element.elementsByTagName("vertex");
  if (vertices.size() < 3) {
    qDebug() << "Cannot create polygon whithin ParserXML::createPolygon: file corrupted";
    return polygon;
  }
  for (int k = 0; k < vertices.size(); k++) {
    polygon << createPoint(vertices.at(k).toElement());
  }

  return polygon;
}

Tape ParserXML::createTape(const QDomElement& element) {
  return Tape(
    element.attribute("x", "-1").toInt(),
    element.attribute("y", "-1").toInt(),
    element.attribute("w", "-1").toInt(),
    element.attribute("h", "-1").toInt()
  );
}

Mirror ParserXML::createMirror(const QDomElement& element) {
  return Mirror(
    element.attribute("xa", "-1").toInt(),
    element.attribute("ya", "-1").toInt(),
    element.attribute("xb", "-1").toInt(),
    element.attribute("yb", "-1").toInt()
  );
}

Hint ParserXML::createHint(const QDomElement& element) {
  QDomNodeList lines = element.elementsByTagName("line");
  QDomNodeList costs = element.elementsByTagName("cost");
  if (lines.size() != 1 || costs.size() != 1) {
    qDebug() << "Cannot create hint whithin ParserXML::createHint: file corrupted";
    return Hint();
  }

  return Hint(createSegment(lines.at(0).toElement()), createInt(costs.at(0).toElement(), "value"));
}


PolygonList ParserXML::createPolygonList(void) {
  PolygonList polygonList;
  for (int k = 0; k < _polygonNodesCount; ++k)
    polygonList << createPolygon(getPolygon(k));

  return polygonList;
}

TapeList ParserXML::createTapeList(void) {
  TapeList tapeList;
  for (int k = 0; k < _tapeNodesCount; ++k)
    tapeList << createTape(getTape(k));

  return tapeList;
}

MirrorList ParserXML::createMirrorList(void) {
  MirrorList mirrorList;
  for (int k = 0; k < _mirrorNodesCount; ++k)
    mirrorList << createMirror(getMirror(k));

  return mirrorList;
}

HintList ParserXML::createHintList(void) {
  HintList hintList;
  for (int k = 0; k < _hintNodesCount; ++k)
    hintList << createHint(getHint(k));

  return hintList;
}

ParserXML& ParserXML::operator=(const ParserXML& parser) {
  _xmlFileName = parser._xmlFileName;
  _doc = parser._doc;

  return (*this);
}

void ParserXML::writeXML(int indent) {
  QFile XMLDoc(_xmlFileName);
  if(!XMLDoc.open(QIODevice::WriteOnly)) {
    qDebug() << "Cannot open XML file in ParserXML::writeXML";
    return;
  }

  QTextStream inFile(&XMLDoc);
  inFile << _doc.toString(indent);

  XMLDoc.close();
}
