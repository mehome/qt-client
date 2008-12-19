/*
 * Common Public Attribution License Version 1.0. 
 * 
 * The contents of this file are subject to the Common Public Attribution 
 * License Version 1.0 (the "License"); you may not use this file except 
 * in compliance with the License. You may obtain a copy of the License 
 * at http://www.xTuple.com/CPAL.  The License is based on the Mozilla 
 * Public License Version 1.1 but Sections 14 and 15 have been added to 
 * cover use of software over a computer network and provide for limited 
 * attribution for the Original Developer. In addition, Exhibit A has 
 * been modified to be consistent with Exhibit B.
 * 
 * Software distributed under the License is distributed on an "AS IS" 
 * basis, WITHOUT WARRANTY OF ANY KIND, either express or implied. See 
 * the License for the specific language governing rights and limitations 
 * under the License. 
 * 
 * The Original Code is xTuple ERP: PostBooks Edition 
 * 
 * The Original Developer is not the Initial Developer and is __________. 
 * If left blank, the Original Developer is the Initial Developer. 
 * The Initial Developer of the Original Code is OpenMFG, LLC, 
 * d/b/a xTuple. All portions of the code written by xTuple are Copyright 
 * (c) 1999-2008 OpenMFG, LLC, d/b/a xTuple. All Rights Reserved. 
 * 
 * Contributor(s): ______________________.
 * 
 * Alternatively, the contents of this file may be used under the terms 
 * of the xTuple End-User License Agreeement (the xTuple License), in which 
 * case the provisions of the xTuple License are applicable instead of 
 * those above.  If you wish to allow use of your version of this file only 
 * under the terms of the xTuple License and not to allow others to use 
 * your version of this file under the CPAL, indicate your decision by 
 * deleting the provisions above and replace them with the notice and other 
 * provisions required by the xTuple License. If you do not delete the 
 * provisions above, a recipient may use your version of this file under 
 * either the CPAL or the xTuple License.
 * 
 * EXHIBIT B.  Attribution Information
 * 
 * Attribution Copyright Notice: 
 * Copyright (c) 1999-2008 by OpenMFG, LLC, d/b/a xTuple
 * 
 * Attribution Phrase: 
 * Powered by xTuple ERP: PostBooks Edition
 * 
 * Attribution URL: www.xtuple.org 
 * (to be included in the "Community" menu of the application if possible)
 * 
 * Graphic Image as provided in the Covered Code, if any. 
 * (online at www.xtuple.com/poweredby)
 * 
 * Display of Attribution Information is required in Larger Works which 
 * are defined in the CPAL as a work which combines Covered Code or 
 * portions thereof with code not governed by the terms of the CPAL.
 */

#include "jsHighlighter.h"

#include <QColor>

#include "format.h"

JSHighlighter::JSHighlighter(QTextDocument *document)
  : QSyntaxHighlighter(document)
{
  _commentColor   = namedColor("altemphasis");
  _errorColor     = namedColor("error");
  _extensionColor = namedColor("warning");
  _keywordColor   = namedColor("emphasis");
  _literalColor   = namedColor("future");

  _keyword  << "break"     << "case"    << "catch"          << "continue"
            << "default"   << "delete"  << "do"             << "else"
            << "false"     << "finally" << "for"            << "function"
            << "if"        << "in"      << "instanceof"     << "new"
            << "null"      << "return"  << "switch"         << "this"
            << "throw"     << "true"    << "try"            << "typeof"
            << "var"       << "void"    << "while"          << "with"
            ;
  _extension  << "abstract" << "boolean"   << "byte"         << "char"
              << "class"    << "const"     << "debugger"     << "double"
              << "enum"     << "export"    << "extends"      << "final"
              << "float"    << "goto"      << "implements"   << "import"
              << "int"      << "interface" << "long"         << "native"
              << "package"  << "private"   << "protected"    << "public"
              << "short"    << "static"    << "super"        << "synchronized"
              << "throws"   << "transient" << "volatile"
              ;
}

JSHighlighter::~JSHighlighter()
{
}

void JSHighlighter::highlightBlock(const QString &text)
{
  int state = previousBlockState();
  int start = 0;

  for (int i = 0; i < text.length(); i++)
  {
    if (state == InsideCStyleComment)
    {
      if (text.mid(i, 2) == "*/")
      {
        state = NormalState;
        setFormat(start, i - start + 2, _commentColor);
      }
    }
    else if (state == InsideString)
    {
      if (text.mid(i, 1) == "\"")
      {
        state = NormalState;
        setFormat(start, i - start + 1, _literalColor);
      }
    }
    else if (text.mid(i, 2) == "//")
    {
      setFormat(i, text.length() - i, _commentColor);
      break;
    }
    else if (text.mid(i, 2) == "/*")
    {
      start = i;
      state = InsideCStyleComment;
    }
    else if (text.mid(i, 1) == "\"")
    {
      start = i;
      state = InsideString;
    }
    else if (text.mid(i - 1, 1).trimmed().isEmpty())
    {
      // TODO: is there a faster way using captured text?
      QRegExp kwtest("^(" + _keyword.join("|") + ")\\W");
      QRegExp extest("^(" + _extension.join("|") + ")\\W");
      if (kwtest.indexIn(text.mid(i)) == 0)
      {
        setFormat(i, kwtest.matchedLength(), _keywordColor);
        i += kwtest.matchedLength();
        break;
      }
      else if (extest.indexIn(text.mid(i)) == 0)
      {
        setFormat(i, extest.matchedLength(), _extensionColor);
        i += extest.matchedLength();
        break;
      }
    }
  }

  if (state == InsideCStyleComment)
    setFormat(start, text.length() - start, _commentColor);
  else if (state == InsideString)
    setFormat(start, text.length() - start, _errorColor);

  setCurrentBlockState(state);
}
