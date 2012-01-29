/**
 *
 * This file is part of Tulip (www.tulip-software.org)
 *
 * Authors: David Auber and the Tulip development Team
 * from LaBRI, University of Bordeaux 1 and Inria Bordeaux - Sud Ouest
 *
 * Tulip is free software; you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License
 * as published by the Free Software Foundation, either version 3
 * of the License, or (at your option) any later version.
 *
 * Tulip is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 * See the GNU General Public License for more details.
 *
 */

#include "PythonInterpreter.h"
#include "PythonScriptViewWidget.h"
#include "PythonScriptView.h"

#include <QtCore/QFile>
#include <QtGui/QToolBar>
#include <QtGui/QIcon>
#include <QtWebKit/QWebView>

GragKeyboardFocusEventFilter PythonScriptViewWidget::keyboardFocusEventFilter;

PythonScriptViewWidget::PythonScriptViewWidget(PythonScriptView *view, QWidget *parent) : QWidget(parent), fontZoom(0) , pythonScriptView(view) {
  setupUi(this);
  consoleOutputWidget->installEventFilter(&keyboardFocusEventFilter);
  mainScriptToolBar = new QToolBar(mainScriptToolBarWidget);
  newMainScriptAction = mainScriptToolBar->addAction(QIcon(":/icons/doc_new.png"), "New main script");
  loadMainScriptAction = mainScriptToolBar->addAction(QIcon(":/icons/doc_import.png"), "Load main script from file");
  saveMainScriptAction = mainScriptToolBar->addAction(QIcon(":/icons/doc_export.png"), "Save main script to file");

  modulesToolBar = new QToolBar(modulesToolBarWidget);
  modulesToolBar->setToolButtonStyle(Qt::ToolButtonIconOnly);
  newStringModuleAction = modulesToolBar->addAction(QIcon(":/icons/doc_plus.png"), "New string module");
  newFileModuleAction = modulesToolBar->addAction(QIcon(":/icons/doc_new.png"), "New file module");
  loadModuleAction = modulesToolBar->addAction(QIcon(":/icons/doc_import.png"), "Import module from file");
  saveModuleAction = modulesToolBar->addAction(QIcon(":/icons/doc_export.png"), "Save module to file");

  pluginsToolBar = new QToolBar(pluginsToolBarWidget);
  pluginsToolBar->setToolButtonStyle(Qt::ToolButtonIconOnly);
  newPluginAction = pluginsToolBar->addAction(QIcon(":/icons/doc_plus.png"), "New Tulip plugin");
  loadPluginAction = pluginsToolBar->addAction(QIcon(":/icons/doc_import.png"), "Import Tulip plugin from file");
  savePluginAction = pluginsToolBar->addAction(QIcon(":/icons/doc_export.png"), "Save Tulip plugin to file");


  modulesTabWidget->clear();
  mainScriptsTabWidget->clear();
  pluginsTabWidget->clear();
  QList<int> sizes;
  sizes.push_back(550);
  sizes.push_back(150);
  splitter->setSizes(sizes);
  splitter->setCollapsible(0, false);
  pluginControlFrame->hide();

  pluginInfoLabel->setTextFormat(Qt::RichText);
  pluginInfoLabel->setText(QString("When the plugin development is finished, you can copy the associated Python file ")
                           + "to <br> <b>" + pythonPluginsPath.c_str() + "</b> or <b> "
                           + pythonPluginsPathHome.c_str() +"</b> <br> and it will be automatically loaded at Tulip startup");
  QFont font;
#ifdef WIN32
  font.setPointSize(7);
#else
  font.setPointSize(6);
#endif
  pluginInfoLabel->setFont(font);

  connect(tabWidget, SIGNAL(currentChanged(int)), this, SLOT(resizeToolBars()));
  connect(decreaseFontSizeButton, SIGNAL(clicked()), this, SLOT(decreaseFontSize()));
  connect(increaseFontSizeButton, SIGNAL(clicked()), this, SLOT(increaseFontSize()));
  connect(decreaseFontSizeButton2, SIGNAL(clicked()), this, SLOT(decreaseFontSize()));
  connect(increaseFontSizeButton2, SIGNAL(clicked()), this, SLOT(increaseFontSize()));
  connect(tabWidget, SIGNAL(currentChanged(int)), this, SLOT(currentTabChanged(int)));

  QString docRootPath = QString(tlp::TulipShareDir.c_str()) + "../doc/tulip-python/html/index.html";

  QFile docRoot(docRootPath);

  if (docRoot.exists()) {
    QWebView *webView = new QWebView();
#ifdef WIN32
    webView->load(QUrl("file:///"+docRootPath));
#else
    webView->load(QUrl("file://"+docRootPath));
#endif
    tabWidget->addTab(webView, "Documentation");
  }

}

std::string PythonScriptViewWidget::getCurrentMainScriptCode() const {
  return static_cast<PythonCodeEditor *>(mainScriptsTabWidget->currentWidget())->toPlainText().replace("\r\n", "\n").toStdString();
}

std::string PythonScriptViewWidget::getMainScriptCode(int idx) const {
  return static_cast<PythonCodeEditor *>(mainScriptsTabWidget->widget(idx))->toPlainText().replace("\r\n", "\n").toStdString();
}

std::string PythonScriptViewWidget::getModuleCode(int idx) const {
  return static_cast<PythonCodeEditor *>(modulesTabWidget->widget(idx))->toPlainText().replace("\r\n", "\n").toStdString();
}

std::string PythonScriptViewWidget::getPluginCode(int idx) const {
  return static_cast<PythonCodeEditor *>(pluginsTabWidget->widget(idx))->toPlainText().replace("\r\n", "\n").toStdString();
}

void PythonScriptViewWidget::resizeEvent(QResizeEvent *e) {
  QWidget::resizeEvent(e);
  resizeToolBars();
}

void PythonScriptViewWidget::showEvent(QShowEvent *e) {
  QWidget::showEvent(e);
  resizeToolBars();
}

void PythonScriptViewWidget::resizeToolBars() {
  modulesToolBar->resize(modulesToolBarWidget->size());
  mainScriptToolBar->resize(mainScriptToolBarWidget->size());
  pluginsToolBar->resize(pluginsToolBarWidget->size());
}

int PythonScriptViewWidget::addMainScriptEditor() {
  PythonCodeEditor *codeEditor = new PythonCodeEditor();
  codeEditor->installEventFilter(pythonScriptView);
  codeEditor->setFocus(Qt::ActiveWindowFocusReason);
  connect(codeEditor, SIGNAL(textChanged()), this, SLOT(mainScriptTextChanged()));
  int idx = mainScriptsTabWidget->addTab(codeEditor, "");
  mainScriptsTabWidget->setCurrentIndex(idx);
  return idx;
}

int PythonScriptViewWidget::addModuleEditor() {
  PythonCodeEditor *codeEditor = new PythonCodeEditor();
  codeEditor->installEventFilter(pythonScriptView);
  codeEditor->setFocus(Qt::ActiveWindowFocusReason);
  connect(codeEditor, SIGNAL(textChanged()), this, SLOT(moduleScriptTextChanged()));
  int idx = modulesTabWidget->addTab(codeEditor, "");
  modulesTabWidget->setCurrentIndex(idx);
  return idx;
}

int PythonScriptViewWidget::addPluginEditor() {
  PythonCodeEditor *codeEditor = new PythonCodeEditor();
  codeEditor->installEventFilter(pythonScriptView);
  codeEditor->setFocus(Qt::ActiveWindowFocusReason);
  connect(codeEditor, SIGNAL(textChanged()), this, SLOT(pluginScriptTextChanged()));
  int idx = pluginsTabWidget->addTab(codeEditor, "");
  pluginsTabWidget->setCurrentIndex(idx);
  return idx;
}

PythonCodeEditor *PythonScriptViewWidget::getCurrentMainScriptEditor() const {
  return static_cast<PythonCodeEditor *>(mainScriptsTabWidget->currentWidget());
}

PythonCodeEditor *PythonScriptViewWidget::getCurrentModuleEditor() const {
  return static_cast<PythonCodeEditor *>(modulesTabWidget->currentWidget());
}

PythonCodeEditor *PythonScriptViewWidget::getMainScriptEditor(int idx) const {
  if (idx < mainScriptsTabWidget->count()) {
    return static_cast<PythonCodeEditor *>(mainScriptsTabWidget->widget(idx));
  }
  else {
    return NULL;
  }
}

PythonCodeEditor *PythonScriptViewWidget::getModuleEditor(int idx) const {
  if (idx < modulesTabWidget->count()) {
    return static_cast<PythonCodeEditor *>(modulesTabWidget->widget(idx));
  }
  else {
    return NULL;
  }
}

PythonCodeEditor *PythonScriptViewWidget::getPluginEditor(int idx) const {
  if (idx < pluginsTabWidget->count()) {
    return static_cast<PythonCodeEditor *>(pluginsTabWidget->widget(idx));
  }
  else {
    return NULL;
  }
}

PythonCodeEditor *PythonScriptViewWidget::getCurrentPluginEditor() const {
  return static_cast<PythonCodeEditor *>(pluginsTabWidget->currentWidget());
}

void PythonScriptViewWidget::decreaseFontSize() {
  for (int i = 0 ; i < mainScriptsTabWidget->count() ; ++i) {
    static_cast<PythonCodeEditor *>(mainScriptsTabWidget->widget(i))->zoomOut();
  }

  for (int i = 0 ; i < modulesTabWidget->count() ; ++i) {
    static_cast<PythonCodeEditor *>(modulesTabWidget->widget(i))->zoomOut();
  }

  for (int i = 0 ; i < pluginsTabWidget->count() ; ++i) {
    static_cast<PythonCodeEditor *>(pluginsTabWidget->widget(i))->zoomOut();
  }

  pythonShellWidget->zoomOut();
  --fontZoom;
}

void PythonScriptViewWidget::increaseFontSize() {
  for (int i = 0 ; i < mainScriptsTabWidget->count() ; ++i) {
    static_cast<PythonCodeEditor *>(mainScriptsTabWidget->widget(i))->zoomIn();
  }

  for (int i = 0 ; i < modulesTabWidget->count() ; ++i) {
    static_cast<PythonCodeEditor *>(modulesTabWidget->widget(i))->zoomIn();
  }

  for (int i = 0 ; i < pluginsTabWidget->count() ; ++i) {
    static_cast<PythonCodeEditor *>(pluginsTabWidget->widget(i))->zoomIn();
  }

  pythonShellWidget->zoomIn();
  ++fontZoom;
}

void PythonScriptViewWidget::mainScriptTextChanged() {
  QString curTabText = mainScriptsTabWidget->tabText(mainScriptsTabWidget->currentIndex());

  if (curTabText == "")
    return;

  if (!curTabText.contains("no file") && curTabText[curTabText.size() -1] != '*') {
    curTabText += "*";
    mainScriptsTabWidget->setTabText(mainScriptsTabWidget->currentIndex(), curTabText);
  }
}

void PythonScriptViewWidget::moduleScriptTextChanged() {
  QString curTabText = modulesTabWidget->tabText(modulesTabWidget->currentIndex());

  if (curTabText == "")
    return;

  if (curTabText[curTabText.size() -1] != '*') {
    curTabText += "*";
    modulesTabWidget->setTabText(modulesTabWidget->currentIndex(), curTabText);
  }
}

void PythonScriptViewWidget::pluginScriptTextChanged() {
  QString curTabText = pluginsTabWidget->tabText(pluginsTabWidget->currentIndex());

  if (curTabText == "")
    return;

  if (curTabText[curTabText.size() -1] != '*') {
    curTabText += "*";
    pluginsTabWidget->setTabText(pluginsTabWidget->currentIndex(), curTabText);
  }
}

void PythonScriptViewWidget::currentTabChanged(int index) {
  static int lastTabIndex = 0;
  static QList<int> lastSizes = splitter->sizes();

  if (lastTabIndex < 2) {
    lastSizes = splitter->sizes();
  }

  QList<int> sizes;

  if (index >= 2) {
    sizes.push_back(height());
    sizes.push_back(0);
    runScriptButton->setEnabled(false);
  }
  else {
    runScriptButton->setEnabled(true);
    sizes = lastSizes;
  }

  splitter->setSizes(sizes);

  if (index >= 3) {
    scriptControlFrame->hide();

    if (index == 3) {
      pluginControlFrame->show();
    }
    else {
      pluginControlFrame->hide();
    }
  }
  else {
    scriptControlFrame->show();
    pluginControlFrame->hide();
  }

  lastTabIndex = index;
}

void PythonScriptViewWidget::setGraph(tlp::Graph *graph) {
  for (int i = 0 ; i < mainScriptsTabWidget->count() ; ++i) {
    static_cast<PythonCodeEditor *>(mainScriptsTabWidget->widget(i))->setGraph(graph);
  }

  for (int i = 0 ; i < modulesTabWidget->count() ; ++i) {
    static_cast<PythonCodeEditor *>(modulesTabWidget->widget(i))->setGraph(graph);
  }

  for (int i = 0 ; i < pluginsTabWidget->count() ; ++i) {
    static_cast<PythonCodeEditor *>(pluginsTabWidget->widget(i))->setGraph(graph);
  }

  pythonShellWidget->setGraph(graph);
}
