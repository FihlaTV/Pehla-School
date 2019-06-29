//
//  LanguageManager.cpp
//  KitkitSchool
//
//  Created by Sungwoo Kang on 6/30/16.
//
//

#include "LanguageManager.hpp"
#include "cocos2d.h"
#include <utility>

USING_NS_CC;
using namespace std;

LanguageManager* LanguageManager::_instance = 0;

LanguageManager* LanguageManager::getInstance()
{
    if(!_instance) {
        _instance = new LanguageManager();
        _instance->init();
    }
    return _instance;
}


void LanguageManager::init()
{
#if false
    auto defaultLang = LanguageType::ENGLISH;
#endif
    //auto defaultLang = LanguageType::SWAHILI;
    auto defaultLang = "sw-tz";//UserDefault::getInstance()->getStringForKey("appLanguage", "en") == "en" ? "en-US" : "sw-TZ";

    auto localeCode = UserDefault::getInstance()->getStringForKey("LocaleCode", defaultLang);

    auto localeType = convertLocaleCodeToType(localeCode);
    if (localeType>=LocaleType_MAX) localeType = sw_TZ;


    _supportedLocales.clear();
    for (int i=0; i<LocaleType_MAX; i++) {
        LocaleType l = (LocaleType)i;
        auto lc = convertLocaleTypeToCode(l);
        auto lp = "Localized/"+lc+"/CurriculumData.tsv";
        if (FileUtils::getInstance()->isFileExist(lp)) _supportedLocales.push_back(l);

    }
    if (_supportedLocales.size() == 0) {
        CCLOGERROR("No curriculumdata.tsv is found for any language. check %s/KitkitSchool/location.txt, which is curretnly refers to %s",
                   FileUtils::getInstance()->getWritablePath().c_str(),
                   FileUtils::getInstance()->getDefaultResourceRootPath().c_str());
        exit(1);
    } else{
        if (std::find(_supportedLocales.begin(), _supportedLocales.end(), localeType)==_supportedLocales.end())
            localeType = _supportedLocales.front();
    }



    setCurrentLocale(localeType);


    initLocalizationMap();



}

LanguageManager::LocaleType LanguageManager::convertLocaleCodeToType(std::string localeCode)
{
    if (localeCode.length()<5) return LocaleType_MAX;
    auto lang = localeCode.substr(0, 2);
    auto region = localeCode.substr(3, 2);

    if (lang=="en") {
        if (region=="US") return en_US;
        if (region=="KE") return en_KE;
        if (region=="GB") return en_GB;
    } else if (lang=="sw") {
        if (region=="TZ") return sw_TZ;
    }

    return LocaleType_MAX;
}

std::string LanguageManager::convertLocaleTypeToCode(LanguageManager::LocaleType localeType)
{
    switch (localeType) {
        case en_US: return "en-US"; break;
        case en_GB: return "en-GB"; break;
        case en_KE: return "en-KE"; break;
        case sw_TZ: return "sw-TZ"; break;
        default: break;
    }

    return "sw-TZ";

}

void LanguageManager::setCurrentLocale(LocaleType type)
{
    bool dirty = false;
    if (_currentLocale != type) dirty = true;


    _currentLocale = type;

    if (dirty) {
        auto langCode = convertLocaleTypeToCode(type);
        UserDefault::getInstance()->setStringForKey("LocaleCode", langCode);
        UserDefault::getInstance()->flush();
    }

    _localizedResourcePaths.clear();
    switch (_currentLocale) {
        default:
            CCLOGERROR("No proper language is found in %s", __PRETTY_FUNCTION__);
            // fall through
        case sw_TZ: _localizedResourcePaths = { "sw-tz" }; break;
        case en_US: _localizedResourcePaths = { "en-us" }; break;
        case en_GB: _localizedResourcePaths = { "en-gb", "en-us" }; break;
        case en_KE: _localizedResourcePaths = { "en-ke", "en-us" }; break;

    }


    std::vector<std::string> paths = {};

    for (auto p : _localizedResourcePaths) {
        auto localizedPath = "localized/"+p;
        paths.push_back(localizedPath);
        paths.push_back(localizedPath+"/games");
    }
    paths.push_back("games");
    paths.push_back("main");


    FileUtils::getInstance()->setSearchPaths(paths);


}

LanguageManager::LocaleType LanguageManager::getCurrentLocaleType()
{
    return _currentLocale;
}

LanguageManager::LocaleType LanguageManager::findNextLocale()
{
    LocaleType next = (LocaleType)((int)_currentLocale+1);

    while (next!=_currentLocale) {
        if (std::find(_supportedLocales.begin(), _supportedLocales.end(), next)!=_supportedLocales.end()) return next;
        next = (LocaleType)((int)next+1);
        if (next>=LocaleType_MAX) next = (LocaleType)0;

    }

    return _currentLocale;
}


std::string LanguageManager::getCurrentLanguageCode()
{

    std::string langCode = convertLocaleTypeToCode(_currentLocale);
    return langCode.substr(0, 2);

}

std::string LanguageManager::getCurrentLanguageTag()
{
    return getCurrentLocaleCode();
}

std::string LanguageManager::getCurrentLocaleCode()
{
    std::string langCode=convertLocaleTypeToCode(_currentLocale);
    return langCode;
}


std::string LanguageManager::soundPathForWordFile(std::string& wordFile)
{
    std::string folder;

//    switch (_langType) {
//        case ENGLISH: folder = "Common/Sounds/Pam.en_US/"; break;
//        case SWAHILI: folder = "Common/Sounds/Imma.sw_TZ/"; break;
//    }

    std::string path = findLocalizedResource("LetterVoice/"+wordFile);
    if (path!="") return path;
    path = findLocalizedResource("WordVoice/"+wordFile);
    if (path!="") return path;

    return "";

}

std::string LanguageManager::getLocalizedString(std::string str)
{

    std::string localized;

    switch (_currentLocale) {
        case en_US: localized = _localizationMapEnglish[str]; break;
        case sw_TZ: localized = _localizationMapSwahili[str]; break;
    }

    if (localized.empty()) return str;

    return localized;

}

std::string LanguageManager::findLocalizedResource(std::string path)
{

    // handled by Cocos...

    return path;

//    
//    for (auto p : _localizedResourcePaths) {
//        auto localizedPath = "Localized/"+p+"/"+path;
//        if (FileUtils::getInstance()->isFileExist(localizedPath)) return localizedPath;
//    }
//
//
//    return "";
}

void LanguageManager::initLocalizationMap()
{
    string delm =  "$#$";
    _localizationMapEnglish["Stop the test"] = "ٹیسٹ روکیں"; // टेस्ट बन्द करिए
    _localizationMapEnglish["Go back to test"] = "ٹیسٹ پر واپس جائیں"; // टेस्ट पर बापस जाइये

    _localizationMapEnglish["Great!"] = "زبردست"; // बहुत अच्छे
    _localizationMapSwahili["Great!"] = "Vizuri!";

    _localizationMapEnglish["Are you ready for"] = "آپ کے لئے تیار ہیں"; // क्या तुम तैयार हो
    _localizationMapSwahili["Are you ready for"] = "Je, uko tayari kwa";

    _localizationMapEnglish["Prove it!"] = "یہ ثابت کرو!"; // साबित करो!
    _localizationMapSwahili["Prove it!"] = "Thibitisha!";

    _localizationMapEnglish["Try and get 8 questions correct!"] = "کوشش کریں اور حاصل 8 سوالات درست!"; // कोशिश करो और आठ प्रशनो का सही जबाब दीजिये
    _localizationMapSwahili["Try and get 8 questions correct!"] = "Jaribu na toa majibu sahihi manane!";

    _localizationMapEnglish["Challenge"] = "چیلنج"; // चुनौती है
    _localizationMapSwahili["Challenge"] = "Jaribu";

    _localizationMapEnglish["Congratulations!"] = "مبارک ہو"; // बधाई हो!
    _localizationMapSwahili["Congratulations!"] = "Hongera!";

    _localizationMapEnglish["You passed!"] = "تم گزر گئے!"; // तुम पास हो गये
    _localizationMapSwahili["You passed!"] = "Umefaulu!";

    _localizationMapEnglish["You failed"] = "آپ ناکام"; // तुम असफल रहे
    _localizationMapSwahili["You failed"] = "Umeshindwa";

    _localizationMapEnglish["Practice more and try again later."] = "مزید عمل کریں اور بعد میں دوبارہ کوشش کیجئے ۔"; // अधिक अभ्यास करें और बाद में फिर से प्रयास करें।
    _localizationMapSwahili["Practice more and try again later."] = "Fanya mazoezi zaidi na rudi baadae.";

    _localizationMapEnglish["Success!"] = "کامیابی!"; // सफलता!
    _localizationMapSwahili["Success!"] = "Mafanikio!";

    _localizationMapEnglish["You are not ready."] =  "آپ تیار نہیں ہیں."; // तुम तैयार नहीं हो।
    _localizationMapSwahili["You are not ready."] = "Hauko tayari.";

    _localizationMapEnglish["You need more practice."] = "آپ کو مزید پریکٹس کی ضرورت ہے."; // आपको और अभ्यास की आवश्यकता है।
    _localizationMapSwahili["You need more practice."] = "Unahitaji mazoezi zaidi.";

    _localizationMapEnglish["Welcome!"] = "خوش آمدید!"; // स्वागत है!
    _localizationMapSwahili["Welcome!"] = "Karibu!";

    _localizationMapEnglish["Start"] = "شروع"; // शुरु करो
    _localizationMapSwahili["Start"] = "Anza";

    _localizationMapEnglish["Next"] = "اگلا"; // आगे
    _localizationMapSwahili["Next"] = "Nenda mbele";

    _localizationMapEnglish["Back"] = "واپس"; // पीछे
    _localizationMapSwahili["Back"] = "Rudi nyuma";

    _localizationMapEnglish["OK"] = "ٹھيک ہے"; // ओके
    _localizationMapSwahili["OK"] = "OK";

    _localizationMapEnglish["Error"] = "نقص"; // गलती
    _localizationMapSwahili["Error"] = "Hitilafu";

    _localizationMapEnglish["Enter"] = "داخل"; // लिखना प्रारम्भ करें
    _localizationMapSwahili["Enter"] = "Chomeka";

    _localizationMapEnglish["Clear"] = "واضح"; // साफ करे
    _localizationMapSwahili["Clear"] = "Futa";

    _localizationMapEnglish["English"] = "انگريزی"; // अंग्रेज़ी
    _localizationMapSwahili["English"] = "Kiswahili";

    _localizationMapEnglish["Math"] = "ریاضی"; // गणित
    _localizationMapSwahili["Math"] = "Hesabu";

    _localizationMapEnglish["TutorialTrace"] = "ٹوٹوراالٹراکا" + delm + "Line Tracing"; // लाइन खीचें
    _localizationMapSwahili["TutorialTrace"] = "Kufuatisha Mstari";

    _localizationMapEnglish["FindTheMatch"] = "فانڈٹہیماٹچ" + delm + "Find The Match"; // जोड़ी खोजिये
    _localizationMapSwahili["FindTheMatch"] = "Tafuta Sare";

    _localizationMapEnglish["NumberMatching"] = "نومبرماٹچانگ" + delm + "Number Matching"; // संख्या मिलाना
    _localizationMapSwahili["NumberMatching"] = "Kufananisha Nambari";

    _localizationMapEnglish["Tapping"] = "ٹیپ" + delm + "Bubble Pop"; // बबल पॉप
    _localizationMapSwahili["Tapping"] = "Pasua Povu la Sabuni";

    _localizationMapEnglish["LetterMatching"] = "لیٹرماٹچانگ" + delm + "Letter Matching"; // अक्षर मिलाना
    _localizationMapSwahili["LetterMatching"] = "Kufananisha Kusoma na Kuandika";

    _localizationMapEnglish["AnimalPuzzle"] = "انامالپوززلی" + delm + "Animal Puzzle"; // पशुओं की पहेली
    _localizationMapSwahili["AnimalPuzzle"] = "Fumbo la Picha";

    _localizationMapEnglish["PatternTrain"] = "پاٹرنٹراان" + delm + "Pattern Train"; // पैटर्न ट्रैन
    _localizationMapSwahili["PatternTrain"] = "Reli ya Garimoshi";

    _localizationMapEnglish["Video"] = "ویڈیو"; // वीडियो
    _localizationMapSwahili["Video"] = "Video";

    _localizationMapEnglish["Counting"] = "گنتی" + delm + "Counting"; // गिनती
    _localizationMapSwahili["Counting"] = "Kuhesabu";

    _localizationMapEnglish["EquationMaker"] = "اقواٹاونماکر" + delm + "Equation Maker"; // समीकरण बनाने वाला
    _localizationMapSwahili["EquationMaker"] = "Kiumba Mlinganyo";

    _localizationMapEnglish["NumberTrain"] = "نومبرٹرین" + delm + "Number Train"; // संख्या ट्रेन
    _localizationMapSwahili["NumberTrain"] = "Nambari ya Garimoshi";

    _localizationMapEnglish["AlphabetPuzzle"] = "الپحبیٹپوززلی" + delm + "Alphabet Puzzle"; // वर्णमाला की पहेली
    _localizationMapSwahili["AlphabetPuzzle"] = "Fumbo la Alfabeti";

    _localizationMapEnglish["Book"] = "کتاب"; // किताब
    _localizationMapSwahili["Book"] = "Kitabu";

    _localizationMapEnglish["Comprehension"] = "فہم" + delm + "Comprehension"; // समझ बूझ
    _localizationMapSwahili["Comprehension"] = "Maswali ya Ufahamu";

    _localizationMapEnglish["DoubleDigit"] = "دووبلاداگاٹ" + delm + "Double Digit Math"; // दो अंको की गणित
    _localizationMapSwahili["DoubleDigit"] = "Hisabati ya Tarakimu Mbili";

    _localizationMapEnglish["FishTank"] = "فاشتانک" + delm + "Fish Tank"; // मछली घर
    _localizationMapSwahili["FishTank"] = "Tangi ya Samaki";

    _localizationMapEnglish["HundredPuzzle"] = "ہونڈرادپوززلی" + delm + "100 Puzzle"; // 100 पहेली
    _localizationMapSwahili["HundredPuzzle"] = "Fumbo la Nambari 100";

    _localizationMapEnglish["LetterTrace"] = "حرف ٹریس" + delm + "Letter Trace"; // अक्षर बनाएं
    _localizationMapSwahili["LetterTrace"] = "Kufuatisha Herufi";

    _localizationMapEnglish["MovingInsects"] = "کیڑوں کی گنتی" + delm + "Bug Math"; // कीड़े की गिनती
    _localizationMapSwahili["MovingInsects"] = "Mchezo wa Mdudu";

    _localizationMapEnglish["SentenceMaker"] = "جملہ میکر" + delm + "Sentence Maker"; // वाक्य बनाने वाला
    _localizationMapSwahili["SentenceMaker"] = "Kiumba Sentensi";

    _localizationMapEnglish["ShapeMatching"] = "شکل مشابہ" + delm + "Shape Matching"; // आकार मिलाना
    _localizationMapSwahili["ShapeMatching"] = "Kufananisha Maumbo";

    _localizationMapEnglish["SoundTrain"] = "آواز کی ٹرین" + delm + "Sound Train"; // ध्वनि ट्रेन
    _localizationMapSwahili["SoundTrain"] = "Sauti ya Garimoshi";

    _localizationMapEnglish["Spelling"] = "ہجے" + delm + "Spelling"; // वर्तनी
    _localizationMapSwahili["Spelling"] = "Matamshi";

    _localizationMapEnglish["WordTracing"] = "لفظ کا سراغ لگانا" + delm + "Word Tracing"; // शब्द अनुरेखण
    _localizationMapSwahili["WordTracing"] = "Kufuatisha Maneno";

    _localizationMapEnglish["NumberTracing"] = "10 کا شمار" + delm + "Number Tracing"; // संख्या अनुरेखण
    _localizationMapSwahili["NumberTracing"] = "Jifunze Mpaka 10";

    _localizationMapEnglish["StarFall"] = "ٹائپ" + delm + "Typing"; // टाइपिंग
    _localizationMapSwahili["StarFall"] = "Kuchapa";

    _localizationMapEnglish["WordMachine"] = "لفظ مشین" + delm + "Word Machine"; // शब्दों की मशीन
    _localizationMapSwahili["WordMachine"] = "Mashine ya Maneno";

    _localizationMapEnglish["NumberTracingExt"] = "نمبر سراغ لگانا" + delm + "Number Tracing"; // संख्या अनुरेखण
    _localizationMapSwahili["NumberTracingExt"] = "Kufuatisha Nambari";

    _localizationMapEnglish["LetterTracingCard"] = "ٹریس 3 اوقات" + delm + "Letter Tracing Card"; // अक्षर ट्रेसिंग कार्ड
    _localizationMapSwahili["LetterTracingCard"] = "Fuatisha Mara 3";

    _localizationMapEnglish["NumberPuzzle"] = "نمبر معما" + delm + "Number Blocks"; // संख्यायों की पहेलियाँ
    _localizationMapSwahili["NumberPuzzle"] = "Fumbo la Nambari";

    _localizationMapEnglish["Arrange the numbers in order from smallest to largest"] = "نمبروں کو سب سے بڑے سے چھوٹے سے ترتیب دیں"; // छोटी से लेकर बड़ी अंक के क्रम में संख्याओं की क्रमबद्ध करें|
    _localizationMapSwahili["Arrange the numbers in order from smallest to largest"] = "panga kwa mpangilio kutoka ndogo zaidi kwenda kubwa zaidi";

    _localizationMapEnglish["Largest number"] = "بڑی تعداد"; // सबसे बड़ी संख्या
    _localizationMapSwahili["Largest number"] = "Namba ipi ndiyo kubwa zaidi?";

    _localizationMapEnglish["BirdPhonics"] = "برڈ پہوناکس" + delm + "Bird Phonics"; // पक्षी की ध्वनि
    _localizationMapSwahili["BirdPhonics"] = "Sauti Ndege";

    _localizationMapEnglish["FeedingTime"] = "غذائی وقت" + delm + "Feeding Time"; // खिलाने का समय
    _localizationMapSwahili["FeedingTime"] = "Wakati wa kula";

    _localizationMapEnglish["LineMatching"] = "لائن مشابہ" + delm + "Line Matching"; // रेखा मिलान
    _localizationMapSwahili["LineMatching"] = "Linganisha mistari";

    _localizationMapEnglish["MangoShop"] = "آم کی دکان" + delm + "Mango Shop"; // आम की दुकान
    _localizationMapSwahili["MangoShop"] = "Duka la embe";

    _localizationMapEnglish["MissingNumber"] = "ناموجود عدد" + delm + "Missing Number"; // अनुपस्थित संख्या
    _localizationMapSwahili["MissingNumber"] = "Namba iliyopotea";

    _localizationMapEnglish["ReadingBird"] = "مطالعہ پرندہ" + delm + "Reading Bird"; // पढ़ने वाला पक्षी
    _localizationMapSwahili["ReadingBird"] = "Ndege anayesoma";

    _localizationMapEnglish["WhatIsThis"] = "يہ کيا ہے?" + delm + "What Is This?"; // यह क्या है?
    _localizationMapSwahili["WhatIsThis"] = "Hii ni nini?";

    _localizationMapEnglish["ThirtyPuzzle"] = "تیس پہیلی" + delm + "30 Puzzle"; // 30 पहेली``
    _localizationMapSwahili["ThirtyPuzzle"] = "Panga Namba";

    _localizationMapEnglish["WordNote"] = "لفظ نوٹ" + delm + "Word Note"; // शब्द नोट
    _localizationMapSwahili["WordNote"] = "Tunga neno";

    _localizationMapEnglish["QuickFacts"] = "فوری حقائق" + delm + "Quick Facts"; // ततुरन्त तथ्य
    _localizationMapSwahili["QuickFacts"] = "Ukweli wa Uhakika";

    _localizationMapEnglish["MultiplicationBoard"] = "ضرب بورڈ" + delm + "Multiplication Lamp"; // गुणा पट्ट
    _localizationMapSwahili["MultiplicationBoard"] = "Taa ya Kuzidishia";

    _localizationMapEnglish["WordMatrix"] = "لفظ میٹرکس" + delm + "Word Matrix"; // शब्द आव्यूह
    _localizationMapSwahili["WordMatrix"] = "Chanzo cha Neno";

    _localizationMapEnglish["SentenceBridge"] = "سزا برج" + delm + "Sentence Bridge"; // वाक्यों का पुल
    _localizationMapSwahili["SentenceBridge"] = "Daraja la Sentensi";

    _localizationMapEnglish["WordWindow"] = "لفظ دریچہ" + delm + "Word Window"; // वर्ड विंडो
    _localizationMapSwahili["WordWindow"] = "Dirisha la Neno";

    _localizationMapEnglish["WordKicker"] = "لفظ کاککار" + delm + "Word Kicker"; // शब्द किकर
    _localizationMapSwahili["WordKicker"] = "Mpigo wa Neno";

    _localizationMapEnglish["MathKicker"] = "ریاضی کاککار" + delm + "Math Kicker"; // मैथ्स किकर
    _localizationMapSwahili["MathKicker"] = "Mpigo wa Hisabati";

    _localizationMapEnglish["PlaceValue"] = "جگہ کی قدر" + delm + "Place Value"; // मान रखे
    _localizationMapSwahili["PlaceValue"] = "Fungu la Thamani";

    _localizationMapEnglish["Labeling"] = "لیبلنگ" + delm + "Labeling"; // अंकितक लगाना
    _localizationMapSwahili["Labeling"] = "Pachika Jina";

    _localizationMapEnglish["LRComprehension"] = "فہم" + delm + "Comprehension"; // समझ बूझ
    _localizationMapSwahili["LRComprehension"] = "Ufahamu";

    _localizationMapEnglish["BookwithQuiz"] = "کتاب مع کوئز"; // प्रश्नोत्तरी के साथ बुक करें
    _localizationMapSwahili["BookwithQuiz"] = "Kitabu Chenye Jaribio";

    _localizationMapEnglish["Do you want to take a test on this egg?"] = "کیا آپ اس انڈے پر ایک امتحان لینا چاہتے ہیں ؟"; // क्या आप इस अंडे पर टेस्ट लेना चाहते हैं?
    _localizationMapSwahili["Do you want to take a test on this egg?"] = "Je, unataka kufanya jaribio kuhusu hili yai?";

    _localizationMapEnglish["Take the quiz to add me to your sea world!"] = "مجھے اپنے سمندر کی دنیا میں شامل کرنے کے لئے کوئز لے لو!"; // मुझे अपनी समुद्री दुनिया में जोड़ने के लिए प्रश्नोत्तरी लो!
    _localizationMapSwahili["Take the quiz to add me to your sea world!"] = "Fanya jaribio ili uniongeze kwenye\ndunia yako ya bahari!";

    _localizationMapEnglish["Congratulations!\nSee you at your sea world!"] = "مبارک ہو! آپ سمندر دنیا میں دیکھیں!"; // बधाई हो! आपकी समुद्री दुनिया में मिलते है!
    _localizationMapSwahili["Congratulations!\nSee you at your sea world!"] = "Hongera!\nTuonane kwenye\ndunia yako ya bahari!";

    _localizationMapEnglish["Try again to add me to your sea world!"] = "مجھے اپنے سمندر کی دنیا میں شامل کرنے کے لئے دوبارہ کوشش کریں!"; // अपनी समुद्री दुनिया में मुझे जोड़ने के लिए फिर से कोशिश करें!
    _localizationMapSwahili["Try again to add me to your sea world!"] = "Jaribu tena kuniongeza kwenye\ndunia yako ya bahari!";

    _localizationMapEnglish["Don't give up! Let's try it again!"] = "ترک مت کرو! چلو اسے دوبارہ کوشش کریں!"; // हार मत मानो! चलो फिर से कोशिश करो!
    _localizationMapSwahili["Don't give up! Let's try it again!"] = "Usikate tamaa! Jaribu tena!";

    // NB(xenosoz, 2018): Migrated from ShapeMatching. Datasheet? I agree.
    vector<pair<string, string>> words_enUS_swTZ = {
            /*
            {"circle", "duara"},
            {"square", "mraba"},
            {"triangle", "pembe_tatu"},
            {"rectangle", "mstatili"},
            {"star", "nyota"},
            {"rhombus", "rombasi"},
            {"diamond", "almasi"},
            {"ova", "mviringo"},
            {"hexagon", "pembe_sita"},
            {"pentagon", "pembe_tano"},
            {"trapezoid", "trapeza"},
            {"parallelogram", "msambamba"},
            {"octagon", "pembe_nane"},
            {"cone", "pia"},
            {"sphere", "nyanja"},
            {"cylinder", "mcheduara"},
            {"cube", "mche_mraba"},
            {"rectangular_prism", "mche_mstatili"},
            {"triangular_prism", "mche_pembe_tatu"},
            {"pyramid", "piramidi"},
             */
            {"चेहरा", "uso"},
            {"चेहरे के", "nyuso"},
            {"पक्ष", "upande"},
            {"पक्षों", "pande"},
            {"विशाल", "kubwa"},
            {"मध्यम", "wastani"},
            {"छोटा", "dogo"}
    };

    for (auto item : words_enUS_swTZ) {
        string enUS = item.first;
        string swTZ = item.second;

        auto key = enUS;
        _localizationMapEnglish[key] = enUS;
        _localizationMapSwahili[key] = swTZ;
    }

}

std::vector<std::string> LanguageManager::getLocalizationMapKeys() {
    std::vector<std::string> rt;
    rt.clear();
    for (auto it: _localizationMapEnglish) {
        rt.push_back(it.first);
    }
    return rt;
}

bool LanguageManager::isSignLanguageMode()
{
    //return true;
    auto ret = UserDefault::getInstance()->getBoolForKey("sign_language_mode_on", false);
    return ret;
}