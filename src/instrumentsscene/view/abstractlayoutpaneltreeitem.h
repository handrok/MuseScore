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

#pragma once

#include <QObject>
#include <QVariant>
#include <QList>

#include "instrumentsscenetypes.h"

#include "notation/imasternotation.h"
#include "notation/iexcerptnotation.h"

namespace mu::instrumentsscene {
struct MoveParams {
    muse::IDList childIdListToMove;
    muse::ID destinationParentId;
    notation::INotationParts::InsertMode insertMode = notation::INotationParts::InsertMode::Before;
};

class AbstractLayoutPanelTreeItem : public QObject
{
    Q_OBJECT

    Q_PROPERTY(QString id READ idStr CONSTANT)
    Q_PROPERTY(QString title READ title WRITE setTitle NOTIFY titleChanged)
    Q_PROPERTY(int type READ typeInt NOTIFY typeChanged)
    Q_PROPERTY(bool isVisible READ isVisible WRITE setIsVisible NOTIFY isVisibleChanged)
    Q_PROPERTY(bool isExpandable READ isExpandable NOTIFY isExpandableChanged)
    Q_PROPERTY(bool isEditable READ isEditable NOTIFY isEditableChanged)
    Q_PROPERTY(bool isRemovable READ isRemovable NOTIFY isRemovableChanged)
    Q_PROPERTY(bool isSelectable READ isSelectable CONSTANT)
    Q_PROPERTY(bool isSelected READ isSelected NOTIFY isSelectedChanged)

public:
    AbstractLayoutPanelTreeItem(LayoutPanelItemType::ItemType type, notation::IMasterNotationPtr masterNotation,
                                notation::INotationPtr notation, QObject* parent);
    virtual ~AbstractLayoutPanelTreeItem();

    muse::ID id() const;
    QString idStr() const;
    QString title() const;
    int typeInt() const;
    LayoutPanelItemType::ItemType type() const;
    bool isVisible() const;
    bool isExpandable() const;
    bool isEditable() const;
    bool isRemovable() const;

    bool isSelectable() const;
    bool isSelected() const;

    Q_INVOKABLE virtual bool canAcceptDrop(const QVariant& item) const;
    Q_INVOKABLE virtual void appendNewItem();

    virtual MoveParams buildMoveParams(int sourceRow, int count, AbstractLayoutPanelTreeItem* destinationParent, int destinationRow) const;

    virtual void moveChildren(int sourceRow, int count, AbstractLayoutPanelTreeItem* destinationParent, int destinationRow,
                              bool updateNotation);

    virtual void removeChildren(int row, int count = 1, bool deleteChild = false);

    AbstractLayoutPanelTreeItem* parentItem() const;
    void setParentItem(AbstractLayoutPanelTreeItem* parent);

    AbstractLayoutPanelTreeItem* childAtId(const muse::ID& id) const;
    AbstractLayoutPanelTreeItem* childAtRow(int row) const;
    const QList<AbstractLayoutPanelTreeItem*>& childItems() const;

    void appendChild(AbstractLayoutPanelTreeItem* child);
    void insertChild(AbstractLayoutPanelTreeItem* child, int beforeRow);

    bool isEmpty() const;
    int childCount() const;
    int row() const;

public slots:
    void setType(LayoutPanelItemType::ItemType type);
    void setTitle(QString title);
    void setIsVisible(bool isVisible, bool setChildren = true);
    void setId(const muse::ID& id);
    void setIsExpandable(bool expandable);
    void setIsEditable(bool editable);
    void setIsRemovable(bool removable);
    void setIsSelectable(bool selectable);
    void setIsSelected(bool selected);

signals:
    void typeChanged(LayoutPanelItemType::ItemType type);
    void titleChanged(QString title);
    void isVisibleChanged(bool isVisible);
    void isExpandableChanged(bool isExpandable);
    void isEditableChanged(bool isEditable);
    void isRemovableChanged(bool isRemovable);
    void isSelectableChanged(bool isSelectable);
    void isSelectedChanged(bool isSelected);

protected:
    notation::IMasterNotationPtr masterNotation() const;
    notation::INotationPtr notation() const;

private:
    int indexOf(const AbstractLayoutPanelTreeItem* item) const;

    QList<AbstractLayoutPanelTreeItem*> m_children;
    AbstractLayoutPanelTreeItem* m_parent = nullptr;

    muse::ID m_id;
    QString m_title;
    LayoutPanelItemType::ItemType m_type = LayoutPanelItemType::ItemType::UNDEFINED;
    bool m_isVisible = false;
    bool m_isExpandable = false;
    bool m_isEditable = false;
    bool m_isRemovable = false;
    bool m_isSelectable = false;
    bool m_isSelected = false;

    notation::IMasterNotationPtr m_masterNotation = nullptr;
    notation::INotationPtr m_notation = nullptr;
};
}
