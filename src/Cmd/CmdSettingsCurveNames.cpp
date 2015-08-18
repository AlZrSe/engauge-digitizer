#include "CmdSettingsCurveNames.h"
#include "CurveNameList.h"
#include "Document.h"
#include "DocumentSerialize.h"
#include "Logger.h"
#include "MainWindow.h"
#include <QXmlStreamReader>

const QString CMD_DESCRIPTION ("Curves settings");

CmdSettingsCurveNames::CmdSettingsCurveNames(MainWindow &mainWindow,
                                             Document &document,
                                             const CurveNameList &modelCurves) :
  CmdAbstract(mainWindow,
              document,
              CMD_DESCRIPTION)
{
  LOG4CPP_INFO_S ((*mainCat)) << "CmdSettingsCurveNames::CmdSettingsCurveNames";

  m_curvesGraphsBefore = document.curvesGraphs ();

  // Build the 'after' state
  for (int row = 0; row < modelCurves.rowCount (); row++) {

    QModelIndex idxCurrent = modelCurves.index (row, 0);
    QModelIndex idxOriginal = modelCurves.index (row, 1);

    QString curveNameCurrent = modelCurves.data (idxCurrent).toString ();
    QString curveNameOriginal = modelCurves.data (idxOriginal).toString ();
    if (!curveNameOriginal.isEmpty ()) {

      // There was an original Curve
      const Curve *curveOriginal = ((const Document&) document).curveForCurveName (curveNameOriginal);
      Curve curveCurrent (*curveOriginal);
      curveCurrent.setCurveName (curveNameCurrent);

      m_curvesGraphsAfter.addGraphCurveAtEnd (curveCurrent); // Save Curve

    } else {

      // There was no original Curve
      Curve curveCurrent (curveNameCurrent,
                          ColorFilterSettings::defaultFilter(),
                          CurveStyle (LineStyle::defaultGraphCurve(m_curvesGraphsAfter.numCurves()),
                                      PointStyle::defaultGraphCurve(m_curvesGraphsAfter.numCurves())));

      m_curvesGraphsAfter.addGraphCurveAtEnd (curveCurrent); // Save Curve
    }
  }
}

CmdSettingsCurveNames::CmdSettingsCurveNames (MainWindow &mainWindow,
                                              Document &document,
                                              const QString &cmdDescription,
                                              QXmlStreamReader &reader) :
  CmdAbstract (mainWindow,
               document,
               cmdDescription)
{
  LOG4CPP_INFO_S ((*mainCat)) << "CmdSettingsCurveNames::CmdSettingsCurveNames";
  
  m_curvesGraphsBefore.loadXml (reader);
  m_curvesGraphsAfter.loadXml (reader);
}

CmdSettingsCurveNames::~CmdSettingsCurveNames ()
{
}

void CmdSettingsCurveNames::cmdRedo ()
{
  LOG4CPP_INFO_S ((*mainCat)) << "CmdSettingsCurveNames::cmdRedo";

  mainWindow().updateSettingsCurveNames(m_curvesGraphsAfter);
  mainWindow().updateAfterCommand();
}

void CmdSettingsCurveNames::cmdUndo ()
{
  LOG4CPP_INFO_S ((*mainCat)) << "CmdSettingsCurveNames::cmdUndo";

  mainWindow().updateSettingsCurveNames(m_curvesGraphsBefore);
  mainWindow().updateAfterCommand();
}

void CmdSettingsCurveNames::saveXml (QXmlStreamWriter &writer) const
{
  writer.writeStartElement(DOCUMENT_SERIALIZE_CMD);
  writer.writeAttribute(DOCUMENT_SERIALIZE_CMD_TYPE, DOCUMENT_SERIALIZE_CMD_SETTINGS_CURVE_NAMES);
  writer.writeAttribute(DOCUMENT_SERIALIZE_CMD_DESCRIPTION, QUndoCommand::text ());
  m_curvesGraphsBefore.saveXml(writer);
  m_curvesGraphsAfter.saveXml(writer);
  writer.writeEndElement();
}