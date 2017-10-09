/*
 * Copyright (c) 2011-2015 BlackBerry Limited.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include <bb/cascades/Application>
#include <bb/cascades/QmlDocument>
#include <bb/cascades/AbstractPane>
#include <bb/cascades/LocaleHandler>

#include "applicationui.hpp"
#include "settings.hpp"
#include "database.hpp"


using namespace bb::cascades;

ApplicationUI :: ApplicationUI() : QObject(), m_dataModel(0) {
    // prepare the localization
    m_pTranslator = new QTranslator(this);
    m_pLocaleHandler = new LocaleHandler(this);
    settings = new Settings(this);
    database = new Database(this);

    if (isFirstStart()) { initializeApplication(); }

    readApplicationSettings();
    readCodeList();

    bool res = QObject::connect(m_pLocaleHandler, SIGNAL(systemLanguageChanged()), this, SLOT(onSystemLanguageChanged()));
    // This is only available in Debug builds
    Q_ASSERT(res);
    // Since the variable is not used in the app, this is added to avoid a
    // compiler warning
    Q_UNUSED(res);

    // initial load
    onSystemLanguageChanged();

    // Create scene document from main.qml asset, the parent is set
    // to ensure the document gets destroyed properly at shut down.
    QmlDocument *qml = QmlDocument::create("asset:///pages/MainPage.qml").parent(this);

    // Create root object for the UI
    AbstractPane *root = qml->createRootObject<AbstractPane>();

    // Set created root object as the application scene
    Application::instance()->setScene(root);

    qml->setContextProperty("_settings", settings);
    qml->setContextProperty("_database", database);
    qml->setContextProperty("_app", this);
}

void ApplicationUI :: onSystemLanguageChanged() {
    QCoreApplication::instance()->removeTranslator(m_pTranslator);
    // Initiate, load and install the application translation files.
    QString locale_string = QLocale().name();
    QString file_name = QString("GoogleAuthenticator_%1").arg(locale_string);
    if (m_pTranslator->load(file_name, "app/native/qm")) {
        QCoreApplication::instance()->installTranslator(m_pTranslator);
    }
}

bool ApplicationUI :: isFirstStart() {
    return settings->isFirstStart();
}

bool ApplicationUI :: initializeApplication() {
    bool success = false;
    if (database->initializeDatabase()){
        if (settings->initializeSettings()){
            success = true;
        }
    }
    database->createRecord();
    return success;
}

bool ApplicationUI :: readApplicationSettings() {
    bool success = false;
    return success;
}

void ApplicationUI :: initializeDataModel()
{
    m_dataModel = new GroupDataModel(this);
    m_dataModel->setSortingKeys(QStringList() << "title");
    m_dataModel->setGrouping(ItemGrouping::None);
}

bool ApplicationUI :: readCodeList() {
    bool success = false;
    initializeDataModel();
    m_dataModel->clear();
    QVariant result = database->readRecords();
    if (!result.isNull()) {
        QVariantList list = result.value<QVariantList>();
        int recordsListSize = list.size();
        for (int i = 0; i < recordsListSize; i++) {
            QVariantMap map = list.at(i).value<QVariantMap>();
            //CodeListItem *listItem = new CodeListItem();
            //m_dataModel->insert(listItem);
        }
        success = true;
    }
    return success;
}

GroupDataModel* ApplicationUI :: getDataModel() const
{
    return m_dataModel;
}
