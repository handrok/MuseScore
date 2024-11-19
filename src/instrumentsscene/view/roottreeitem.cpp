/*
 * SPDX-License-Identifier: GPL-3.0-only
 * MuseScore-Studio-CLA-applies
 *
 * MuseScore Studio
 * Music Composition & Notation
 *
 * Copyright (C) 2024 MuseScore Limited
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 3 as
 * published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */
#include "roottreeitem.h"

#include "systemobjectslayertreeitem.h"

using namespace mu::instrumentsscene;
using namespace mu::notation;
using namespace muse;

static inline bool isIndexInRange(int index, int start, int end)
{
    return start <= index && index < end;
}

RootTreeItem::RootTreeItem(IMasterNotationPtr masterNotation, INotationPtr notation, QObject* parent)
    : AbstractLayoutPanelTreeItem(LayoutPanelItemType::ItemType::ROOT, masterNotation, notation, parent)
{
}

MoveParams RootTreeItem::buildMoveParams(int sourceRow, int count, AbstractLayoutPanelTreeItem* destinationParent,
                                         int destinationRow) const
{
    MoveParams moveParams;

    IDList partIds;

    for (int i = sourceRow; i < sourceRow + count; ++i) {
        ID partId = childAtRow(i)->id();

        if (notation()->parts()->partExists(partId)) {
            partIds.push_back(partId);
        }
    }

    moveParams.childIdListToMove = partIds;

    int destinationRow_ = destinationRow;
    int childCount = destinationParent->childCount();
    bool moveDown = destinationRow > sourceRow;
    INotationParts::InsertMode moveMode = INotationParts::InsertMode::Before;
    const AbstractLayoutPanelTreeItem* destinationPartItem = nullptr;

    do {
        destinationPartItem = destinationParent->childAtRow(destinationRow_);

        if (destinationPartItem) {
            if (notation()->parts()->partExists(destinationPartItem->id())) {
                break;
            }

            destinationPartItem = nullptr;
        }

        if (moveDown) {
            destinationRow_--;
            moveMode = INotationParts::InsertMode::After;
        } else {
            destinationRow_++;
            moveMode = INotationParts::InsertMode::Before;
        }
    } while (isIndexInRange(destinationRow_, 0, childCount) && !isIndexInRange(destinationRow_, sourceRow, sourceRow + count));

    if (destinationPartItem) {
        moveParams.destinationParentId = destinationPartItem->id();
    }
    moveParams.insertMode = moveMode;

    return moveParams;
}

void RootTreeItem::moveChildren(int sourceRow, int count, AbstractLayoutPanelTreeItem* destinationParent,
                                int destinationRow, bool updateNotation)
{
    if (updateNotation) {
        MoveParams moveParams = buildMoveParams(sourceRow, count, destinationParent, destinationRow);
        if (moveParams.destinationParentId.isValid()) {
            notation()->parts()->moveParts(moveParams.childIdListToMove, moveParams.destinationParentId, moveParams.insertMode);
        }
    }
    AbstractLayoutPanelTreeItem::moveChildren(sourceRow, count, destinationParent, destinationRow, updateNotation);
}

void RootTreeItem::removeChildren(int row, int count, bool deleteChild)
{
    IDList partIds;
    IDList stavesIds;

    for (int i = row; i < row + count; ++i) {
        const AbstractLayoutPanelTreeItem* child = childAtRow(i);

        if (child->type() == LayoutPanelItemType::ItemType::PART) {
            partIds.push_back(child->id());
        } else if (child->type() == LayoutPanelItemType::ItemType::SYSTEM_OBJECTS_LAYER) {
            stavesIds.push_back(child->id());
        }
    }

    if (deleteChild) {
        masterNotation()->notation()->parts()->removeParts(partIds);
        masterNotation()->notation()->parts()->removeSystemObjects(stavesIds);
    }

    AbstractLayoutPanelTreeItem::removeChildren(row, count, deleteChild);
}
