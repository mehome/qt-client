/*
 * This file is part of the xTuple ERP: PostBooks Edition, a free and
 * open source Enterprise Resource Planning software suite,
 * Copyright (c) 1999-2012 by OpenMFG LLC, d/b/a xTuple.
 * It is licensed to you under the Common Public Attribution License
 * version 1.0, the full text of which (including xTuple-specific Exhibits)
 * is available at www.xtuple.com/CPAL.  By using this software, you agree
 * to be bound by its terms.
 */

#ifndef PRINTLABELSBYORDER_H
#define PRINTLABELSBYORDER_H

#include "guiclient.h"
#include "xdialog.h"
#include <parameter.h>
#include "ui_printLabelsByOrder.h"

class printLabelsByOrder : public XDialog, public Ui::printLabelsByOrder
{
    Q_OBJECT

public:
    printLabelsByOrder(QWidget* parent = 0, const char* name = 0, bool modal = false, Qt::WFlags fl = 0);
    ~printLabelsByOrder();

public slots:
    virtual enum SetResponse set(const ParameterList & pParams );
    virtual void sPrint();
    virtual void sSetToMin( int pValue );

protected slots:
    virtual void languageChange();

private:
    bool _captive;

};

#endif // PRINTLABELSBYORDER_H
