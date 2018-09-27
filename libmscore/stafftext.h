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

#ifndef __STAFFTEXT_H__
#define __STAFFTEXT_H__

#include "text.h"
#include "part.h"
#include "staff.h"
#include "stafftextbase.h"

namespace Ms {

//---------------------------------------------------------
//   StaffText
//---------------------------------------------------------

class StaffText : public StaffTextBase  {
   public:
      StaffText(Score* s = 0, Tid = Tid::STAFF);
      virtual StaffText* clone() const override       { return new StaffText(*this); }
      virtual ElementType type() const override       { return ElementType::STAFF_TEXT; }
      virtual void layout() override;
      virtual QVariant propertyDefault(Pid id) const override;
      };

enum class TripletFillType {
      Triplet8th,
      Triplet16th,
      Dotted8th,
      Dotted16th,
      Scottish8th,
      Scottish16th,
      None
      };

class TripletFill final : public StaffText  {
      constexpr static int eghtDivision = 2;
      constexpr static int sixteenthDivision = 4;
      constexpr static int tripletRatio = 66;
      constexpr static int dottedRatio = 75;
      constexpr static int scottishRatio = 25;

      TripletFillType _tripletFillType;
      
   public:
      TripletFill(Score* s = nullptr, TripletFillType tripletFillType = TripletFillType::None) : StaffText(s) {
            _tripletFillType = tripletFillType;
            setTripletProperty();
            }
      void setTripletProperty() {
            setSwing(true);

            switch (_tripletFillType) {
            case TripletFillType::Triplet8th:
                  setSwingParameters(MScore::division / eghtDivision, tripletRatio);
                  setXmlText("Triplet 8th");
                  break;
            case TripletFillType::Triplet16th:
                  setSwingParameters(MScore::division / sixteenthDivision, tripletRatio);
                  setXmlText("Triplet 16th");
                  break;
            case TripletFillType::Dotted8th:
                  setSwingParameters(MScore::division / eghtDivision, dottedRatio);
                  setXmlText("Dotted 8th");
                  break;
            case TripletFillType::Dotted16th:
                  setSwingParameters(MScore::division / sixteenthDivision, dottedRatio);
                  setXmlText("Dotted 16th");
                  break;
            case TripletFillType::Scottish8th:
                  setSwingParameters(MScore::division / eghtDivision, scottishRatio);
                  setXmlText("Scottish 18th");
                  break;
            case TripletFillType::Scottish16th:
                  setSwingParameters(MScore::division / sixteenthDivision, scottishRatio);
                  setXmlText("Scottish 16th");
                  break;
            case TripletFillType::None:
                  setSwing(false);
                  setXmlText("None Triplet Feel");
                  break;
                  }
            }
      };

}     // namespace Ms
#endif
