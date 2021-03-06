/******************************************************************************************************
 * (C) 2014 markummitchell@github.com. This file is part of Engauge Digitizer, which is released      *
 * under GNU General Public License version 2 (GPLv2) or (at your option) any later version. See file *
 * LICENSE or go to gnu.org/licenses for details. Distribution requires prior written permission.     *
 ******************************************************************************************************/

#ifndef TRANSFORMATION_STATE_UNDEFINED_H
#define TRANSFORMATION_STATE_UNDEFINED_H

#include "TransformationStateAbstractBase.h"

/// Class to not show transformation since transformation is undefined.
class TransformationStateUndefined : public TransformationStateAbstractBase
{
public:
  /// Single constructor.
  TransformationStateUndefined(TransformationStateContext &context,
                               QGraphicsScene &scene);

  virtual void begin(bool isGnuplot,
                     CmdMediator &cmdMediator,
                     const Transformation &transformation,
                     const QString &selectedGraphCurve);
  virtual void end(CmdMediator &cmdMediator,
                   const Transformation &transformation);
  virtual void updateAxesChecker (CmdMediator &cmdMediator,
                                  const Transformation &transformation);

private:
  TransformationStateUndefined();
};

#endif // TRANSFORMATION_STATE_UNDEFINED_H
