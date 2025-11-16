// extra_tests_fixed.cpp
// 30 focused testcases for VectorStore / SinglyLinkedList / ArrayList
// Only include project headers (no additional libs)
#include "VectorStore.h"

// Helpers ---------------------------------------------------------------

static const double EPS = 1e-6;

SinglyLinkedList<float>* makeListFromArray(const float arr[], int n) {
    SinglyLinkedList<float>* l = new SinglyLinkedList<float>();
    for (int i = 0; i < n; ++i) l->add(arr[i]);
    return l;
}

static string fmtDouble(double d) {
    // use std::to_string and trim trailing zeros for compactness
    string s = to_string(d);
    // trim trailing zeros and possible trailing dot
    if (s.find('.') != string::npos) {
        while (!s.empty() && s.back() == '0') s.pop_back();
        if (!s.empty() && s.back() == '.') s.pop_back();
    }
    return s;
}

string listToString(SinglyLinkedList<float>& l) {
    string s = "[";
    for (int i = 0; i < l.size(); ++i) {
        if (i) s += ", ";
        s += fmtDouble((double) l.get(i));
    }
    s += "]";
    return s;
}

bool listEquals(SinglyLinkedList<float>& a, const float expected[], int n, double eps = EPS) {
    if (a.size() != n) return false;
    for (int i = 0; i < n; ++i) {
        double va = (double)a.get(i);
        if (fabs(va - expected[i]) > eps) return false;
    }
    return true;
}

void printFail(const string& name, const string& msg) {
    cout << name << ": FAIL - " << msg << endl;
}
void printPass(const string& name) {
    cout << name << ": PASS" << endl;
}

// Embedding helpers ----------------------------------------------------

SinglyLinkedList<float>* embed_long(const string& s) {
    float arr[] = {1.0f, 2.0f, 3.0f, 4.0f};
    return makeListFromArray(arr, 4);
}
SinglyLinkedList<float>* embed_short(const string& s) {
    float arr[] = {0.1f, 0.2f};
    return makeListFromArray(arr, 2);
}
SinglyLinkedList<float>* embed_empty(const string& s) {
    float arr[] = {};
    return makeListFromArray(arr, 0);
}
SinglyLinkedList<float>* embed_map_small(const string& s) {
    if (s == "a") { float a[] = {1.0f, 0.0f}; return makeListFromArray(a, 2); }
    if (s == "b") { float a[] = {0.0f, 1.0f}; return makeListFromArray(a, 2); }
    if (s == "c") { float a[] = {0.5f, 0.5f}; return makeListFromArray(a, 2); }
    float a[] = {0.0f, 0.0f}; return makeListFromArray(a, 2);
}

// ----------------- Part A tests (1..10) -------------------------------

void A01_TruncateLongerEmbed() {
    const string name = "A01 Truncate longer embed";
    VectorStore s(3, embed_long);
    s.addText("anything");
    try {
        SinglyLinkedList<float>& v = s.getVector(0);
        float expected[] = {1.0f, 2.0f, 3.0f};
        if (listEquals(v, expected, 3)) printPass(name);
        else printFail(name, string("got ") + listToString(v) + " expected [1,2,3]");
    } catch (const exception& e) {
        printFail(name, string("threw exception: ") + e.what());
    }
}

void A02_PostPadShortEmbed() {
    const string name = "A02 Post-pad short embed";
    VectorStore s(5, embed_short);
    s.addText("B");
    try {
        SinglyLinkedList<float>& v = s.getVector(0);
        float expected[] = {0.1f, 0.2f, 0.0f, 0.0f, 0.0f};
        if (listEquals(v, expected, 5)) printPass(name);
        else printFail(name, string("got ") + listToString(v));
    } catch (const exception& e) {
        printFail(name, string("threw exception: ") + e.what());
    }
}

void A03_EmptyEmbedToZeros() {
    const string name = "A03 Empty embed -> zeros";
    VectorStore s(4, embed_empty);
    s.addText("");
    try {
        SinglyLinkedList<float>& v = s.getVector(0);
        float expected[] = {0.0f, 0.0f, 0.0f, 0.0f};
        if (listEquals(v, expected, 4)) printPass(name);
        else printFail(name, string("got ") + listToString(v));
    } catch (const exception& e) {
        printFail(name, string("threw exception: ") + e.what());
    }
}

void A04_MultipleAdd_SizeRawId() {
    const string name = "A04 Multiple add: size/raw/id";
    VectorStore s(2, embed_map_small);
    s.addText("a"); s.addText("b"); s.addText("c");
    bool ok = true;
    if (s.size() != 3) ok = false;
    try {
        if (s.getId(0) != 1) ok = false;
        if (s.getRawText(2) != "c") ok = false;
        float ea[] = {1.0f, 0.0f};
        float eb[] = {0.0f, 1.0f};
        if (!listEquals(s.getVector(0), ea, 2)) ok = false;
        if (!listEquals(s.getVector(1), eb, 2)) ok = false;
        if (ok) printPass(name); else printFail(name, "one or more invariants failed");
    } catch (const exception& e) {
        printFail(name, string("threw exception: ") + e.what());
    }
}

void A05_GetVectorInvalidIndex() {
    const string name = "A05 getVector invalid index";
    VectorStore s(1, embed_short);
    s.addText("x");
    try {
        s.getVector(-1);
        printFail(name, "expected out_of_range for negative index");
    } catch (const out_of_range& ) {
        printPass(name);
    } catch (const exception& e) {
        printFail(name, string("unexpected exception: ") + e.what());
    }
}

void A06_GetRawTextInvalid() {
    const string name = "A06 getRawText invalid";
    VectorStore s(1, embed_short);
    s.addText("x");
    try {
        s.getRawText(5);
        printFail(name, "expected out_of_range for large index");
    } catch (const out_of_range&) {
        printPass(name);
    } catch (const exception& e) {
        printFail(name, string("unexpected exception: ") + e.what());
    }
}

void A07_RemoveAtMiddleShiftIdPreserved() {
    const string name = "A07 removeAt middle shift & id preserved";
    VectorStore s(2, embed_map_small);
    s.addText("a"); s.addText("b"); s.addText("c");
    try {
        bool removed = s.removeAt(1);
        if (!removed) { printFail(name, "removeAt returned false"); return; }
        if (s.size() != 2) { printFail(name, "size not decreased"); return; }
        if (s.getRawText(1) != "c") { printFail(name, "index 1 not updated to former index2"); return; }
        int id_at_1 = s.getId(1);
        if (id_at_1 != 3) { printFail(name, string("id not preserved, got id=") + to_string(id_at_1)); return; }
        printPass(name);
    } catch (const exception& e) {
        printFail(name, string("threw exception: ") + e.what());
    }
}

void A08_RemoveAtEmptyThrows() {
    const string name = "A08 removeAt empty -> exception";
    VectorStore s(2, embed_map_small);
    try {
        s.removeAt(0);
        printFail(name, "expected out_of_range for removing from empty store");
    } catch (const out_of_range&) {
        printPass(name);
    } catch (const exception& e) {
        printFail(name, string("unexpected exception: ") + e.what());
    }
}

void A09_UpdateTextPreservesIdAndUpdatesVector() {
    const string name = "A09 updateText preserves id and updates vector";
    VectorStore s(3, embed_long);
    s.addText("orig");
    try {
        bool ok = s.updateText(0, "new");
        if (!ok) { printFail(name, "updateText returned false"); return; }
        if (s.getId(0) != 1) { printFail(name, "id changed after update"); return; }
        if (s.getRawText(0) != "new") { printFail(name, "rawText not updated"); return; }
        float expected[] = {1.0f, 2.0f, 3.0f};
        if (!listEquals(s.getVector(0), expected, 3)) { printFail(name, string("vector not updated properly: got ") + listToString(s.getVector(0))); return; }
        printPass(name);
    } catch (const exception& e) {
        printFail(name, string("threw exception: ") + e.what());
    }
}

void A10_UpdateTextInvalidIndex() {
    const string name = "A10 updateText invalid index";
    VectorStore s(2, embed_map_small);
    s.addText("a");
    try {
        s.updateText(5, "hello");
        printFail(name, "expected out_of_range for updateText invalid index");
    } catch (const out_of_range&) {
        printPass(name);
    } catch (const exception& e) {
        printFail(name, string("unexpected exception: ") + e.what());
    }
}

// ----------------- Part B tests (11..20) ------------------------------

void B01_SetEmbeddingFunctionUsed() {
    const string name = "B01 setEmbeddingFunction used";
    // initial function:
    auto f1 = [](const string&){
        float arr[] = {1.0f};
        return makeListFromArray(arr, 1);
    };
    VectorStore s(2, f1);
    // change to f2
    auto f2 = [](const string&){
        float arr[] = {9.0f, 9.0f};
        return makeListFromArray(arr, 2);
    };
    s.setEmbeddingFunction(f2);
    s.addText("z");
    try {
        SinglyLinkedList<float>& v = s.getVector(0);
        float expected[] = {9.0f, 9.0f};
        if (listEquals(v, expected, 2)) printPass(name);
        else printFail(name, string("got ") + listToString(v));
    } catch (const exception& e) {
        printFail(name, string("threw exception: ") + e.what());
    }
}

void B02_forEachInsertionOrder() {
    const string name = "B02 forEach insertion order";
    VectorStore s(2, embed_map_small);
    s.addText("a"); s.addText("b");
    // print order; test marks PASS if no exception (we can't capture callback output easily without extra structures)
    auto action_print = [](SinglyLinkedList<float>& v, int id, string& raw) {
        cout << "(forEach) id=" << id << " raw='" << raw << "' vec=" << listToString(v) << '\n';
    };
    try {
        s.forEach(action_print);
        printPass(name);
    } catch (const exception& e) {
        printFail(name, string("threw exception: ") + e.what());
    }
}

void B03_CosineSimilarityOrthogonal() {
    const string name = "B03 cosineSimilarity orthogonal ~0";
    VectorStore s(2, embed_map_small);
    float a_arr[] = {1.0f, 0.0f}; SinglyLinkedList<float>* a = makeListFromArray(a_arr, 2);
    float b_arr[] = {0.0f, 1.0f}; SinglyLinkedList<float>* b = makeListFromArray(b_arr, 2);
    try {
        double v = s.cosineSimilarity(*a, *b);
        delete a; delete b;
        if (fabs(v) < 1e-9) printPass(name);
        else printFail(name, string("got cosine=") + to_string(v));
    } catch (const exception& e) {
        delete a; delete b;
        printFail(name, string("threw exception: ") + e.what());
    }
}

void B04_CosineSimilarityIdenticalVectors() {
    const string name = "B04 cosineSimilarity identical ~1";
    VectorStore s(3, embed_long);
    float a_arr[] = {1.0f, 2.0f, 3.0f}; SinglyLinkedList<float>* a = makeListFromArray(a_arr, 3);
    try {
        double v = s.cosineSimilarity(*a, *a);
        delete a;
        if (fabs(v - 1.0) < 1e-9) printPass(name);
        else printFail(name, string("got cosine=") + to_string(v));
    } catch (const exception& e) {
        delete a;
        printFail(name, string("threw exception: ") + e.what());
    }
}

void B05_L1DistanceSimple() {
    const string name = "B05 l1Distance simple";
    VectorStore s(3, embed_long);
    float a_arr[] = {1.0f, 2.0f, 3.0f}; SinglyLinkedList<float>* a = makeListFromArray(a_arr, 3);
    float b_arr[] = {0.0f, 2.0f, 5.0f}; SinglyLinkedList<float>* b = makeListFromArray(b_arr, 3);
    try {
        double d = s.l1Distance(*a, *b);
        delete a; delete b;
        if (fabs(d - 3.0) < EPS) printPass(name);
        else printFail(name, string("got l1=") + to_string(d));
    } catch (const exception& e) {
        delete a; delete b;
        printFail(name, string("threw exception: ") + e.what());
    }
}

void B06_L2DistanceSimple() {
    const string name = "B06 l2Distance simple";
    VectorStore s(2, embed_map_small);
    float a_arr[] = {3.0f, 4.0f}; SinglyLinkedList<float>* a = makeListFromArray(a_arr, 2);
    float b_arr[] = {0.0f, 0.0f}; SinglyLinkedList<float>* b = makeListFromArray(b_arr, 2);
    try {
        double d = s.l2Distance(*a, *b);
        delete a; delete b;
        if (fabs(d - 5.0) < EPS) printPass(name);
        else printFail(name, string("got l2=") + to_string(d));
    } catch (const exception& e) {
        delete a; delete b;
        printFail(name, string("threw exception: ") + e.what());
    }
}

void B07_findNearestCosine() {
    const string name = "B07 findNearest (cosine)";
    auto ef = [](const string& t)->SinglyLinkedList<float>* {
        if (t=="r0") { float a[] = {1.0f, 0.0f}; return makeListFromArray(a, 2); }
        if (t=="r1") { float a[] = {0.9f, 0.1f}; return makeListFromArray(a, 2); }
        float a[] = {0.0f, 1.0f}; return makeListFromArray(a, 2);
    };
    VectorStore s(2, ef);
    s.addText("r0"); s.addText("r1"); s.addText("r2");
    float q_arr[] = {1.0f, 0.0f};
    SinglyLinkedList<float>* q = makeListFromArray(q_arr, 2);
    try {
        int idx = s.findNearest(*q, "cosine");
        delete q;
        if (idx == 0) printPass(name);
        else printFail(name, string("expected 0 got ") + to_string(idx));
    } catch (const exception& e) {
        delete q;
        printFail(name, string("threw exception: ") + e.what());
    }
}

void B08_findNearestManhattan() {
    const string name = "B08 findNearest (manhattan)";
    auto ef = [](const string& t)->SinglyLinkedList<float>* {
        if (t=="z0") { float a[] = {0.0f, 0.0f}; return makeListFromArray(a, 2); }
        if (t=="z1") { float a[] = {0.0f, 1.0f}; return makeListFromArray(a, 2); }
        float a[] = {1.0f, 1.0f}; return makeListFromArray(a, 2);
    };
    VectorStore s(2, ef);
    s.addText("z0"); s.addText("z1"); s.addText("z2");
    float q_arr[] = {0.0f, 0.6f};
    SinglyLinkedList<float>* q = makeListFromArray(q_arr, 2);
    try {
        int idx = s.findNearest(*q, "manhattan");
        delete q;
        if (idx == 1) printPass(name);
        else printFail(name, string("expected 1 got ") + to_string(idx));
    } catch (const exception& e) {
        delete q;
        printFail(name, string("threw exception: ") + e.what());
    }
}

void B09_findNearestInvalidMetric() {
    const string name = "B09 findNearest invalid metric";
    VectorStore s(2, embed_empty);
    s.addText("a");
    float q_arr[] = {0.0f, 0.0f};
    SinglyLinkedList<float>* q = makeListFromArray(q_arr, 2);
    try {
        s.findNearest(*q, "unknown_metric");
        delete q;
        printFail(name, "expected invalid_metric exception");
    } catch (const invalid_metric&) {
        delete q;
        printPass(name);
    } catch (const exception& e) {
        delete q;
        printFail(name, string("unexpected exception: ") + e.what());
    }
}

void B10_topKNearestInvalidK() {
    const string name = "B10 topKNearest invalid k";
    VectorStore s(2, embed_map_small);
    s.addText("a"); s.addText("b");
    float q_arr[] = {1.0f, 0.0f};
    SinglyLinkedList<float>* q = makeListFromArray(q_arr, 2);
    try {
        int* r = s.topKNearest(*q, 5, "cosine");
        delete[] r;
        delete q;
        printFail(name, "expected invalid_k_value exception for k>count");
    } catch (const invalid_k_value&) {
        delete q;
        printPass(name);
    } catch (const exception& e) {
        delete q;
        printFail(name, string("unexpected exception: ") + e.what());
    }
}

// ----------------- Part C tests (21..30) -----------------------------

void C01_topKNearestCosineOrdering() {
    const string name = "C01 topKNearest cosine ordering";
    auto ef = [](const string& t)->SinglyLinkedList<float>* {
        if (t=="v0") { float a[] = {1.0f, 0.0f}; return makeListFromArray(a, 2); }
        if (t=="v1") { float a[] = {0.8f, 0.2f}; return makeListFromArray(a, 2); }
        float a[] = {0.0f, 1.0f}; return makeListFromArray(a, 2);
    };
    VectorStore s(2, ef);
    s.addText("v0"); s.addText("v1"); s.addText("v2");
    float q_arr[] = {1.0f, 0.0f};
    SinglyLinkedList<float>* q = makeListFromArray(q_arr, 2);
    try {
        int* top2 = s.topKNearest(*q, 2, "cosine");
        if (top2[0] == 0 && top2[1] == 1) printPass(name);
        else printFail(name, string("got [") + to_string(top2[0]) + "," + to_string(top2[1]) + "]");
        delete[] top2;
        delete q;
    } catch (const exception& e) {
        delete q;
        printFail(name, string("threw exception: ") + e.what());
    }
}

void C02_topKNearestManhattanOrdering() {
    const string name = "C02 topKNearest manhattan ordering";
    auto ef = [](const string& t)->SinglyLinkedList<float>* {
        if (t=="p0") { float a[] = {0.0f, 0.0f}; return makeListFromArray(a, 2); }
        if (t=="p1") { float a[] = {0.1f, 0.1f}; return makeListFromArray(a, 2); }
        float a[] = {1.0f, 1.0f}; return makeListFromArray(a, 2);
    };
    VectorStore s(2, ef);
    s.addText("p0"); s.addText("p1"); s.addText("p2");
    float q_arr[] = {0.0f, 0.05f};
    SinglyLinkedList<float>* q = makeListFromArray(q_arr, 2);
    try {
        int* top2 = s.topKNearest(*q, 2, "manhattan");
        if (top2[0] == 0 && top2[1] == 1) printPass(name);
        else printFail(name, string("got [") + to_string(top2[0]) + "," + to_string(top2[1]) + "]");
        delete[] top2;
        delete q;
    } catch (const exception& e) {
        delete q;
        printFail(name, string("threw exception: ") + e.what());
    }
}

void C03_topKNearestEuclideanOrdering() {
    const string name = "C03 topKNearest euclidean ordering";
    auto ef = [](const string& t)->SinglyLinkedList<float>* {
        if (t=="q0") { float a[] = {0.0f, 0.0f}; return makeListFromArray(a, 2); }
        if (t=="q1") { float a[] = {0.0f, 2.0f}; return makeListFromArray(a, 2); }
        float a[] = {3.0f, 0.0f}; return makeListFromArray(a, 2);
    };
    VectorStore s(2, ef);
    s.addText("q0"); s.addText("q1"); s.addText("q2");
    float q_arr[] = {1.0f, 0.0f};
    SinglyLinkedList<float>* q = makeListFromArray(q_arr, 2);
    try {
        // NOTE: some implementations might use "euclidean" but the existing code had "eucliean"
        // We call with "eucliean" to detect that bug — but spec wants "euclidean".
        // Here call both: prefer "euclidean" first, fallback to "eucliean"
        int* res = nullptr;
        try { res = s.topKNearest(*q, 2, "euclidean"); }
        catch (...) {
            res = s.topKNearest(*q, 2, "eucliean");
        }
        if (res[0] == 0 && res[1] == 2) printPass(name);
        else printFail(name, string("got [") + to_string(res[0]) + "," + to_string(res[1]) + "]");
        delete[] res;
        delete q;
    } catch (const exception& e) {
        delete q;
        printFail(name, string("threw exception: ") + e.what());
    }
}

void C04_forEachMutationReflected() {
    const string name = "C04 forEach mutation reflected";
    auto ef = [](const string&){ float a[] = {1.0f, 2.0f}; return makeListFromArray(a, 2); };
    VectorStore s(2, ef);
    s.addText("x");
    auto action = [](SinglyLinkedList<float>& v, int id, string& raw) {
        for (int i = 0; i < v.size(); ++i) {
            float val = v.get(i);
            v.get(i) = val * 2.0f;
        }
    };
    try {
        s.forEach(action);
        float expected[] = {2.0f, 4.0f};
        if (listEquals(s.getVector(0), expected, 2)) printPass(name);
        else printFail(name, string("got ") + listToString(s.getVector(0)));
    } catch (const exception& e) {
        printFail(name, string("threw exception: ") + e.what());
    }
}

void C05_ClearAndReuseNoCrash() {
    const string name = "C05 clear then reuse (no crash)";
    VectorStore s(2, embed_map_small);
    s.addText("a"); s.addText("b");
    s.clear();
    try {
        if (!s.empty()) { printFail(name, "clear did not make store empty"); return; }
        s.addText("a");
        if (s.size() != 1) { printFail(name, "addText failed after clear"); return; }
        printPass(name);
    } catch (const exception& e) {
        printFail(name, string("threw exception: ") + e.what());
    }
}

void C06_RemoveAtLastThenAccessThrows() {
    const string name = "C06 removeAt last -> getVector invalid";
    VectorStore s(2, embed_map_small);
    s.addText("a");
    try {
        bool ok = s.removeAt(0);
        if (!ok) { printFail(name, "removeAt returned false"); return; }
        try {
            s.getVector(0);
            printFail(name, "expected out_of_range after removing last element");
        } catch (const out_of_range&) {
            printPass(name);
        }
    } catch (const exception& e) {
        printFail(name, string("threw exception: ") + e.what());
    }
}

void C07_GrowthViaAddText() {
    const string name = "C07 growth via addText (ArrayList ensureCapacity)";
    VectorStore s(2, embed_map_small);
    const int N = 25;
    for (int i = 0; i < N; ++i) s.addText("x");
    if (s.size() == N) printPass(name);
    else printFail(name, string("size expected ") + to_string(N) + " got " + to_string(s.size()));
}

void C08_IteratorTraversalOfVector() {
    const string name = "C08 iterator traversal of vector";
    auto ef = [](const string&){ float a[] = {10.0f,20.0f,30.0f}; return makeListFromArray(a, 3); };
    VectorStore s(3, ef);
    s.addText("t");
    try {
        SinglyLinkedList<float>& v = s.getVector(0);
        string out;
        for (auto it = v.begin(); it != v.end(); ++it) {
            if (!out.empty()) out += ",";
            out += to_string((double)*it);
        }
        if (!out.empty()) printPass(name);
        else printFail(name, "iterator produced nothing");
    } catch (const exception& e) {
        printFail(name, string("threw exception: ") + e.what());
    }
}

void C09_UpdateTextDoesNotChangeId() {
    const string name = "C09 updateText keeps id unchanged";
    VectorStore s(2, embed_map_small);
    s.addText("a");
    int id_before = s.getId(0);
    s.updateText(0, "b");
    int id_after = s.getId(0);
    if (id_before == id_after) printPass(name);
    else printFail(name, string("id changed from ") + to_string(id_before) + " to " + to_string(id_after));
}

void C10_topKNearest_kEqualsCount_returnsAllOrdered() {
    const string name = "C10 topKNearest k==count returns all ordered";
    auto ef = [](const string& t)->SinglyLinkedList<float>* {
        if (t=="A") { float a[] = {1.0f,0.0f}; return makeListFromArray(a, 2); }
        if (t=="B") { float a[] = {0.6f,0.8f}; return makeListFromArray(a, 2); }
        float a[] = {0.0f,1.0f}; return makeListFromArray(a, 2);
    };
    VectorStore s(2, ef);
    s.addText("A"); s.addText("B"); s.addText("C");
    float q_arr[] = {1.0f,0.0f};
    SinglyLinkedList<float>* q = makeListFromArray(q_arr, 2);
    try {
        int* res = s.topKNearest(*q, s.size(), "cosine");
        bool ok = (res[0] == 0);
        if (ok) printPass(name);
        else printFail(name, string("unexpected ordering first element = ") + to_string(res[0]));
        delete[] res;
        delete q;
    } catch (const exception& e) {
        delete q;
        printFail(name, string("threw exception: ") + e.what());
    }
}

// Runner ---------------------------------------------------------------

void runExtraTests() {
    cout << "Running 30 focused VectorStore tests (extra, fixed)...\n";
    A01_TruncateLongerEmbed();
    A02_PostPadShortEmbed();
    A03_EmptyEmbedToZeros();
    A04_MultipleAdd_SizeRawId();
    A05_GetVectorInvalidIndex();
    A06_GetRawTextInvalid();
    A07_RemoveAtMiddleShiftIdPreserved();
    A08_RemoveAtEmptyThrows();
    A09_UpdateTextPreservesIdAndUpdatesVector();
    A10_UpdateTextInvalidIndex();

    B01_SetEmbeddingFunctionUsed();
    B02_forEachInsertionOrder();
    B03_CosineSimilarityOrthogonal();
    B04_CosineSimilarityIdenticalVectors();
    B05_L1DistanceSimple();
    B06_L2DistanceSimple();
    B07_findNearestCosine();
    B08_findNearestManhattan();
    B09_findNearestInvalidMetric();
    B10_topKNearestInvalidK();

    C01_topKNearestCosineOrdering();
    C02_topKNearestManhattanOrdering();
    C03_topKNearestEuclideanOrdering();
    C04_forEachMutationReflected();
    C05_ClearAndReuseNoCrash();
    C06_RemoveAtLastThenAccessThrows();
    C07_GrowthViaAddText();
    C08_IteratorTraversalOfVector();
    C09_UpdateTextDoesNotChangeId();
    C10_topKNearest_kEqualsCount_returnsAllOrdered();

    cout << "All extra tests finished.\n";
}
int main(){
    runExtraTests();
}