//=============================================================================
//  MuseScore
//  Music Composition & Notation
//
//  Copyright (C) 2002-2011 Werner Schweer
//
//  This program is free software; you can redistribute it and/or modify
//  it under the terms of the GNU General Public License version 2
//  as published by the Free Software Foundation and appearing in
//  the file LICENCE.GPL
//=============================================================================

#ifndef __REPEAT_H__
#define __REPEAT_H__

#include "text.h"
#include "rest.h"

namespace Ms {

class Score;
class Segment;

//---------------------------------------------------------
//   @@ RepeatMeasure
//---------------------------------------------------------

enum class RepeatMeasureType {
	Simple,
	FirstOfDouble,
	SecondOfDouble
};

class RepeatMeasure : public Rest {
   protected:
      QPainterPath path;
      RepeatMeasureType _repeatType { RepeatMeasureType::Simple };

   public:
      RepeatMeasure(Score*);
      RepeatMeasure &operator=(const RepeatMeasure&) = delete;
      virtual RepeatMeasure* clone() const override   { return new RepeatMeasure(*this); }
      virtual Element* linkedClone() override         { return Element::linkedClone(); }
      virtual ElementType type() const override       { return ElementType::REPEAT_MEASURE; }
      virtual void draw(QPainter*) const override;
      virtual void layout() override;
      virtual Fraction duration() const override;
      Fraction actualDuration() const { return Rest::duration(); }

      void setRepeatType(RepeatMeasureType val) { _repeatType = val; }
	  RepeatMeasureType getRepeatType() const { return _repeatType; }

      virtual QString accessibleInfo() const override;
      };

class DoubleRepeatMeasure final : public RepeatMeasure {
      
   public:
      DoubleRepeatMeasure(Score*);
      DoubleRepeatMeasure &operator=(const DoubleRepeatMeasure&) = delete;
      virtual DoubleRepeatMeasure* clone() const override   { return new DoubleRepeatMeasure(*this); }
      virtual Element* linkedClone() override         { return Element::linkedClone(); }
      virtual ElementType type() const override       { return ElementType::DOUBLE_REPEAT_MEASURE; }
//      virtual void draw(QPainter*) const override;
      virtual void layout() override;
//      virtual Fraction duration() const override;
//      Fraction actualDuration() const { return Rest::duration(); }

      void setRepeatType(RepeatMeasureType val) { _repeatType = val; }
	  RepeatMeasureType getRepeatType() const { return _repeatType; }

//      virtual QString accessibleInfo() const override;
      };

}     // namespace Ms
#endif

