/******************************************************************************************************
 * (C) 2019 markummitchell@github.com. This file is part of Engauge Digitizer, which is released      *
 * under GNU General Public License version 2 (GPLv2) or (at your option) any later version. See file *
 * LICENSE or go to gnu.org/licenses for details. Distribution requires prior written permission.     *
 ******************************************************************************************************/

#include "CallbackGatherXThetasInGridLines.h"
#include "Document.h"
#include "EngaugeAssert.h"
#include "ExportAlignLinear.h"
#include "ExportAlignLog.h"
#include "ExportLayoutFunctions.h"
#include "ExportPointsSelectionFunctions.h"
#include "GridLineLimiter.h"
#include "Logger.h"
#include "MainWindowModel.h"
#include "Point.h"
#include <qmath.h>

CallbackGatherXThetasInGridLines::CallbackGatherXThetasInGridLines(const MainWindowModel &modelMainWindow,
                                                                   const DocumentModelExportFormat &modelExport,
                                                                   const QStringList &curvesIncluded,
                                                                   const Transformation &transformation,
                                                                   const Document &document) :
  CallbackGatherXThetasAbstractBase (modelExport,
                                     curvesIncluded,
                                     transformation)
{
  addGridLines (modelMainWindow,
                transformation,
                document);
}

void CallbackGatherXThetasInGridLines::addGridLines (const MainWindowModel &modelMainWindow,
                                                     const Transformation &transformation,
                                                     const Document &document)
{
  DocumentModelGridDisplay gridLines = document.modelGridDisplay();

  // Prevent overflow
  GridLineLimiter gridLineLimiter;
  double startX = document.modelGridDisplay().startX();
  double stepX = document.modelGridDisplay().stepX();
  double stopX = document.modelGridDisplay().stopX();
  gridLineLimiter.limitForXTheta (document,
                                  transformation,
                                  document.modelCoords(),
                                  modelMainWindow,
                                  document.modelGridDisplay(),
                                  startX,
                                  stepX,
                                  stopX);
  if (document.modelCoords().coordScaleXTheta() == COORD_SCALE_LINEAR) {
    // Linear
    unsigned int countX = (unsigned int) (0.5 + 1 + (stopX - startX) / stepX);
    for (unsigned int i = 0; i < countX; i++) {
      double x = startX + i * stepX;
      addGraphX (x);
    }
  } else {
    // Log
    unsigned int countX = 1.0 + (qLn (stopX) - qLn (startX)) / qLn (stepX);
    for (unsigned int i = 0; i < countX; i++) {
      double x = startX * qPow (stepX, i);
      addGraphX (x);
    }
  }
}

CallbackSearchReturn CallbackGatherXThetasInGridLines::callback (const QString &curveName,
                                                                 const Point &point)
{
  LOG4CPP_DEBUG_S ((*mainCat)) << "CallbackGatherXThetasInGridLines::callback"
                               << " curveName=" << curveName.toLatin1().data()
                               << " point=" << point.identifier().toLatin1().data();

  // Skip everything unless the endpoints are to be collected
  if (m_exportEndpoints == EXPORT_ENDPOINTS_INCLUDE) {

    if (curvesIncludedHash ().contains (curveName)) {

      QPointF posGraph;
      transformation ().transformScreenToRawGraph (point.posScreen(),
                                                   posGraph);

      if (!m_curveLimitsMin.contains (curveName) ||
          posGraph.x() < m_curveLimitsMin [curveName]) {

        m_curveLimitsMin [curveName] = posGraph.x ();
      }

      if (!m_curveLimitsMax.contains (curveName) ||
          posGraph.x() > m_curveLimitsMax [curveName]) {

        m_curveLimitsMax [curveName] = posGraph.x ();
      }
    }
  }

  return CALLBACK_SEARCH_RETURN_CONTINUE;
}

void CallbackGatherXThetasInGridLines::finalize ()
{
  LOG4CPP_INFO_S ((*mainCat)) << "CallbackGatherXThetasInGridLines::finalize";

  // Merge any saved endpoints
  CurveLimits::const_iterator itr;
  for (itr = m_curveLimitsMin.begin(); itr != m_curveLimitsMin.end(); itr++) {
    double value = *itr;
    addGraphX (value);
  }
  for (itr = m_curveLimitsMax.begin(); itr != m_curveLimitsMax.end(); itr++) {
    double value = *itr;
    addGraphX (value);
  }
}
