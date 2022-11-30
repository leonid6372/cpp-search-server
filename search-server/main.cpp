// -------- Начало модульных тестов поисковой системы ----------

// Тест проверяет, что поисковая система исключает стоп-слова при добавлении документов
void TestExcludeStopWordsFromAddedDocumentContent() {
    const int doc_id = 42;
    const string content = "cat in the city"s;
    const vector<int> ratings = { 1, 2, 3 };
    // Сначала убеждаемся, что поиск слова, не входящего в список стоп-слов,
    // находит нужный документ
    {
        SearchServer server;
        server.AddDocument(doc_id, content, DocumentStatus::ACTUAL, ratings);
        const auto found_docs = server.FindTopDocuments("in"s);
        ASSERT_EQUAL(found_docs.size(), 1);
        const Document& doc0 = found_docs[0];
        ASSERT_EQUAL(doc0.id, doc_id);
    }

    // Затем убеждаемся, что поиск этого же слова, входящего в список стоп-слов,
    // возвращает пустой результат
    {
        SearchServer server;
        server.SetStopWords("in the"s);
        server.AddDocument(doc_id, content, DocumentStatus::ACTUAL, ratings);
        ASSERT(server.FindTopDocuments("in"s).empty());
    }
}

void TestDocumentStatus() {
    SearchServer server;
    server.AddDocument(0, "Charles Leclerc"s, DocumentStatus::IRRELEVANT, {});
    server.AddDocument(1, "what a beautiful takeover from Charles"s, DocumentStatus::ACTUAL, { 1, 1, 1 });
    server.AddDocument(2, "Charles Leclerc finished last lap in this race"s, DocumentStatus::BANNED, { 2, 2, 2 });
    server.AddDocument(3, "and Leclerc is in pole position for tomorrow's race"s, DocumentStatus::REMOVED, { 3, 3, 3 });
    
    vector<Document> result = server.FindTopDocuments("Leclerc", DocumentStatus::IRRELEVANT);
    ASSERT_EQUAL(result.at(0).id, 0);
    result = server.FindTopDocuments("takeover", DocumentStatus::ACTUAL);
    ASSERT_EQUAL(result.at(0).id, 1);
    result = server.FindTopDocuments("race", DocumentStatus::BANNED);
    ASSERT_EQUAL(result.at(0).id, 2);
    result = server.FindTopDocuments("race", DocumentStatus::REMOVED);
    ASSERT_EQUAL(result.at(0).id, 3);
}

void TestPredicate() {
    SearchServer server;
    server.AddDocument(0, ""s, DocumentStatus::ACTUAL, {});
    server.AddDocument(1, "what a beautiful takeover from Charles"s, DocumentStatus::ACTUAL, { 1, 1, 1 });
    server.AddDocument(2, "Charles Leclerc finished last lap in this race"s, DocumentStatus::BANNED, { 2, 2, 2 });
    server.AddDocument(3, "and Leclerc is in pole position for tomorrow's race"s, DocumentStatus::ACTUAL, { 3, 3, 3 });
    const vector<Document> result = server.FindTopDocuments("Leclerc", [](int document_id, DocumentStatus status, int rating) { return document_id % 2 == 0; });
    ASSERT_EQUAL(result.at(0).id, 2);
}

void TestRating() {
    SearchServer server;
    server.AddDocument(0, "Charles Leclerc"s, DocumentStatus::ACTUAL, {});
    server.AddDocument(1, "what a beautiful takeover from Charles"s, DocumentStatus::ACTUAL, { 1, 1, 1 });
    server.AddDocument(2, "Charles Leclerc finished last lap in this race"s, DocumentStatus::ACTUAL, { 2, 2, 2 });
    server.AddDocument(3, "and Leclerc is in pole position for tomorrow's race"s, DocumentStatus::ACTUAL, { 3, 3, 3 });
    const vector<Document> result = server.FindTopDocuments("Charles Leclerc");
    for (const auto& document : result)
        ASSERT_EQUAL(document.id, document.rating);
}

void TestMinusWords() {
    SearchServer server;
    server.AddDocument(0, "Charles Leclerc"s, DocumentStatus::ACTUAL, {});
    server.AddDocument(1, "what a beautiful takeover from Charles"s, DocumentStatus::ACTUAL, { 1, 1, 1 });
    server.AddDocument(2, "Charles Leclerc finished last lap in this race"s, DocumentStatus::ACTUAL, { 2, 2, 2 });
    server.AddDocument(3, "and Leclerc is in pole position for tomorrow's race"s, DocumentStatus::ACTUAL, { 3, 3, 3 });
    const vector<Document> result = server.FindTopDocuments("Charles Leclerc -race");
    for(int i = 0; i < result.size(); ++i)
        ASSERT_EQUAL(result.at(i).id, i);
}

void TestRelevance() {
    SearchServer server;
    server.AddDocument(0, "Charles Leclerc"s, DocumentStatus::ACTUAL, {});
    server.AddDocument(1, "what a beautiful takeover from Charles"s, DocumentStatus::ACTUAL, { 1, 1, 1 });
    server.AddDocument(2, "Charles Leclerc finished last lap in this race"s, DocumentStatus::ACTUAL, { 2, 2, 2 });
    server.AddDocument(3, "and Leclerc is in pole position for tomorrow's race"s, DocumentStatus::ACTUAL, { 3, 3, 3 });
    
    const vector<Document> result = server.FindTopDocuments("Charles Leclerc");
    
    const double EPSILON = 1e-6;
    vector<double> relevance = {0.287682, 0.0719205, 0.047947, 0.0319647};
    for (int i = 0; i < result.size(); ++i)
        ASSERT(abs(result.at(i).relevance - relevance[i]) < EPSILON);
}

// Функция TestSearchServer является точкой входа для запуска тестов
void TestSearchServer() {
    TestExcludeStopWordsFromAddedDocumentContent();
    TestDocumentStatus();
    TestPredicate();
    TestRating();
    TestMinusWords();
    TestRelevance();
    // Не забудьте вызывать остальные тесты здесь
}

// --------- Окончание модульных тестов поисковой системы -----------
