#include "content/ContentFactory.h"

namespace rpg {

void ContentFactory::populateWorld(World& world) {
    auto& loc = world.locationsMutable();
    loc["gorod_svetograd"] = {"gorod_svetograd", "Светоград", LocationType::City, {"les_serebryany", "ruiny_solntsa", "bolota_mgli"}, {"mer", "torgovec", "kapitan", "kuznec"}, {}};
    loc["les_serebryany"] = {"les_serebryany", "Серебряный лес", LocationType::Forest, {"gorod_svetograd", "pesheri_pepla", "pereval_grifona"}, {"ohotnik", "druid", "sledopyt"}, {"wolf", "bandit", "spider", "boar"}};
    loc["ruiny_solntsa"] = {"ruiny_solntsa", "Руины Солнца", LocationType::Ruins, {"gorod_svetograd", "hram_rassveta"}, {"ucheniy", "kontrabandist"}, {"skeleton", "cultist", "gargoyle"}};
    loc["pesheri_pepla"] = {"pesheri_pepla", "Пепельные пещеры", LocationType::Caves, {"les_serebryany", "krepost_tirana"}, {"svyashennik", "shahtyor"}, {"slime", "goblin", "ogre", "bat"}};
    loc["bolota_mgli"] = {"bolota_mgli", "Туманные болота", LocationType::Forest, {"gorod_svetograd", "hram_rassveta"}, {"vedma", "travnitsa"}, {"ghoul", "toad", "bandit_archer"}};
    loc["pereval_grifona"] = {"pereval_grifona", "Перевал Грифона", LocationType::Forest, {"les_serebryany", "krepost_tirana"}, {"naemnik", "razvedchik"}, {"harpy", "yeti", "raider"}};
    loc["hram_rassveta"] = {"hram_rassveta", "Храм Рассвета", LocationType::Dungeon, {"ruiny_solntsa", "bolota_mgli", "krepost_tirana"}, {"orakul", "arhierei"}, {"wraith", "knight", "acolyte"}};
    loc["krepost_tirana"] = {"krepost_tirana", "Крепость Тирана", LocationType::Dungeon, {"pesheri_pepla", "pereval_grifona", "hram_rassveta"}, {"plennik", "komendant"}, {"executioner", "warlock", "boss"}};

    auto& n = world.npcs();
    n["mer"]={"mer","Мэр Торин",Faction::Nobles,0,{"q_main_1","q_side_1"}};
    n["torgovec"]={"torgovec","Мира Торговка",Faction::Guild,0,{"q_side_2"}};
    n["kapitan"]={"kapitan","Капитан Руск",Faction::Guild,0,{"q_faction_1"}};
    n["kuznec"]={"kuznec","Кузнец Браго",Faction::Guild,0,{"q_side_6"}};
    n["ohotnik"]={"ohotnik","Охотник Брам",Faction::Guild,0,{"q_side_3"}};
    n["druid"]={"druid","Друид Эловен",Faction::Church,0,{"q_side_4"}};
    n["sledopyt"]={"sledopyt","Следопыт Каэл",Faction::Guild,0,{"q_random_1"}};
    n["ucheniy"]={"ucheniy","Учёный Венн",Faction::Nobles,0,{"q_main_2"}};
    n["kontrabandist"]={"kontrabandist","Контрабандист Тень",Faction::Bandits,0,{"q_faction_2"}};
    n["svyashennik"]={"svyashennik","Отец Малрик",Faction::Church,0,{"q_side_5"}};
    n["shahtyor"]={"shahtyor","Шахтёр Дагг",Faction::Guild,0,{"q_random_2"}};
    n["vedma"]={"vedma","Болотная ведьма Иара",Faction::Bandits,0,{"q_side_7"}};
    n["travnitsa"]={"travnitsa","Травница Нери",Faction::Church,0,{"q_side_8"}};
    n["naemnik"]={"naemnik","Наёмник Варг",Faction::Nobles,0,{"q_side_9"}};
    n["razvedchik"]={"razvedchik","Разведчик Эйр",Faction::Guild,0,{"q_side_10"}};
    n["orakul"]={"orakul","Слепой Оракул",Faction::Church,0,{"q_main_3"}};
    n["arhierei"]={"arhierei","Архиерей Лиан",Faction::Church,0,{"q_faction_3"}};
    n["plennik"]={"plennik","Пленный Рыцарь",Faction::Nobles,0,{"q_main_4"}};
    n["komendant"]={"komendant","Комендант Гарр",Faction::Bandits,0,{"q_faction_4"}};

    auto addEnemy=[&](std::string id,std::string name,int hp,int ac,int atk,int d,int spd,int xp,int gold){
        world.enemies()[id]={id,name,{hp,hp,ac,atk,d,spd,8},xp,gold,{"iron_sword","health_potion","bandage"}};
    };
    addEnemy("wolf","Лютый волк",45,11,3,6,12,25,10);
    addEnemy("bandit","Разбойник",55,12,4,8,11,30,12);
    addEnemy("spider","Ядовитый паук",40,12,5,6,13,28,14);
    addEnemy("boar","Бешеный кабан",52,12,4,8,10,30,13);
    addEnemy("skeleton","Древний скелет",58,13,4,8,8,35,16);
    addEnemy("cultist","Культист пепла",50,12,5,10,10,38,18);
    addEnemy("gargoyle","Гаргулья",68,14,6,10,9,50,20);
    addEnemy("slime","Кислотный слизень",64,10,4,6,7,30,14);
    addEnemy("goblin","Пещерный гоблин",47,11,5,6,12,32,15);
    addEnemy("ogre","Пещерный огр",100,12,6,10,6,50,22);
    addEnemy("bat","Кровавая летучая мышь",44,11,5,6,14,34,16);
    addEnemy("ghoul","Болотный гуль",75,13,6,10,9,54,22);
    addEnemy("toad","Ядовитая жаба",62,11,5,8,8,45,18);
    addEnemy("bandit_archer","Разбойник-лучник",58,12,6,8,13,46,19);
    addEnemy("harpy","Гарпия",80,14,6,10,14,62,25);
    addEnemy("yeti","Снежный йети",120,14,8,12,7,80,34);
    addEnemy("raider","Горный налётчик",88,13,7,10,11,70,30);
    addEnemy("wraith","Ночной призрак",75,14,6,10,14,60,25);
    addEnemy("knight","Падший рыцарь",95,15,7,12,10,75,30);
    addEnemy("acolyte","Тёмный аколит",82,14,7,10,12,72,32);
    addEnemy("executioner","Палач Тирана",130,15,8,12,9,100,45);
    addEnemy("warlock","Чернокнижник",110,15,9,12,11,110,50);
    addEnemy("boss","Малгор Тиран",200,17,10,12,12,300,200);

    auto addItem=[&](std::string id,std::string name,ItemType t,Rarity r,int val,double w,int p,bool c=false){ world.items()[id]={id,name,t,r,val,w,p,c};};
    addItem("iron_sword","Железный меч",ItemType::Weapon,Rarity::Common,20,4,2);
    addItem("steel_sword","Стальной меч",ItemType::Weapon,Rarity::Uncommon,40,4.5,4);
    addItem("oak_staff","Дубовый посох",ItemType::Weapon,Rarity::Common,18,3,3);
    addItem("moon_dagger","Лунный кинжал",ItemType::Weapon,Rarity::Rare,65,1.5,5);
    addItem("warhammer","Боевой молот",ItemType::Weapon,Rarity::Rare,70,6,6);
    addItem("hunter_bow","Лук охотника",ItemType::Weapon,Rarity::Uncommon,48,2.5,4);
    addItem("cloth_armor","Тканый доспех",ItemType::Armor,Rarity::Common,15,3,1);
    addItem("chainmail","Кольчуга",ItemType::Armor,Rarity::Uncommon,45,8,3);
    addItem("knight_plate","Латы рыцаря",ItemType::Armor,Rarity::Epic,120,12,5);
    addItem("shadow_cloak","Плащ теней",ItemType::Armor,Rarity::Rare,85,2,3);
    addItem("ring_focus","Кольцо концентрации",ItemType::Ring,Rarity::Rare,80,0.1,2);
    addItem("ring_guard","Кольцо стража",ItemType::Ring,Rarity::Uncommon,55,0.1,2);
    addItem("ring_venom","Кольцо яда",ItemType::Ring,Rarity::Epic,120,0.1,3);
    addItem("health_potion","Зелье лечения",ItemType::Potion,Rarity::Common,15,0.5,30,true);
    addItem("mana_tonic","Тоник маны",ItemType::Potion,Rarity::Uncommon,18,0.5,0,true);
    addItem("elixir_guard","Эликсир защиты",ItemType::Potion,Rarity::Rare,45,0.8,0,true);
    addItem("antidote","Противоядие",ItemType::Potion,Rarity::Common,14,0.4,0,true);
    addItem("fire_bomb","Огненная бомба",ItemType::Potion,Rarity::Rare,35,1.0,22,true);
    addItem("scroll_fire","Свиток огня",ItemType::Scroll,Rarity::Uncommon,30,0.2,10,true);
    addItem("scroll_frost","Свиток льда",ItemType::Scroll,Rarity::Rare,45,0.2,0,true);
    addItem("bandage","Полевой бинт",ItemType::Potion,Rarity::Common,8,0.3,12,true);
    addItem("lockpick","Набор отмычек",ItemType::Quest,Rarity::Common,10,0.2,0);
    addItem("sun_shard","Осколок солнца",ItemType::Quest,Rarity::Epic,0,0.4,0);
    addItem("oracle_seal","Печать Оракула",ItemType::Quest,Rarity::Legendary,0,0.1,0);
    addItem("tyrant_crown","Корона Тирана",ItemType::Quest,Rarity::Legendary,0,1.0,0);
}

void ContentFactory::populateQuests(QuestSystem& quests) {
    quests.addQuest({"q_main_1","Главный: Тени над Светоградом",QuestType::Main,{{"Поговорить с мэром",false},{"Зачистить Серебряный лес",false}},false,1,{{Faction::Nobles,2},{Faction::Guild,1}}});
    quests.addQuest({"q_main_2","Главный: Тайны Руин Солнца",QuestType::Main,{{"Найти учёного Венна",false},{"Добыть Осколок солнца",false}},false,1,{{Faction::Nobles,1}}});
    quests.addQuest({"q_main_3","Главный: Голос Рассвета",QuestType::Main,{{"Посетить Храм Рассвета",false},{"Получить Печать Оракула",false}},false,1,{{Faction::Church,2}}});
    quests.addQuest({"q_main_4","Главный: Корона Тирана",QuestType::Main,{{"Проникнуть в Крепость Тирана",false},{"Победить Малгора",false}},false,2,{{Faction::Church,2},{Faction::Guild,2}}});
    quests.addQuest({"q_side_1","Побочный: Пропавший караван",QuestType::Side,{{"Найти следы каравана",false},{"Спасти выживших",false}},false,1,{{Faction::Guild,1}}});
    quests.addQuest({"q_side_2","Побочный: Нехватка припасов",QuestType::Side,{{"Добыть лечебные травы",false}},false,0,{{Faction::Guild,1}}});
    quests.addQuest({"q_side_3","Побочный: Охота на хищника",QuestType::Side,{{"Убить лютого волка",false}},false,0,{{Faction::Guild,1}}});
    quests.addQuest({"q_side_4","Побочный: Очищение рощи",QuestType::Side,{{"Сжечь заражённые корни",false}},false,1,{{Faction::Church,1}}});
    quests.addQuest({"q_side_5","Побочный: Последний обряд",QuestType::Side,{{"Вернуть святую реликвию",false}},false,1,{{Faction::Church,2}}});
    quests.addQuest({"q_side_6","Побочный: Сталь для кузнеца",QuestType::Side,{{"Принести руду из пещер",false}},false,0,{{Faction::Guild,1}}});
    quests.addQuest({"q_side_7","Побочный: Сделка с ведьмой",QuestType::Side,{{"Собрать болотные грибы",false}},false,-1,{{Faction::Bandits,1}}});
    quests.addQuest({"q_side_8","Побочный: Лекарь болота",QuestType::Side,{{"Доставить противоядие",false}},false,1,{{Faction::Church,1}}});
    quests.addQuest({"q_side_9","Побочный: Долг наёмника",QuestType::Side,{{"Проверить перевал",false}},false,0,{{Faction::Nobles,1}}});
    quests.addQuest({"q_side_10","Побочный: Карта руин",QuestType::Side,{{"Найти древнюю карту",false}},false,0,{{Faction::Guild,1}}});
    quests.addQuest({"q_faction_1","Фракция: Клятва Гильдии",QuestType::Faction,{{"Помочь городскому дозору",false}},false,0,{{Faction::Guild,3}}});
    quests.addQuest({"q_faction_2","Фракция: Контрабандный путь",QuestType::Faction,{{"Провести груз через руины",false}},false,-1,{{Faction::Bandits,3},{Faction::Nobles,-1}}});
    quests.addQuest({"q_faction_3","Фракция: Дело Церкви",QuestType::Faction,{{"Защитить паломников",false}},false,1,{{Faction::Church,3}}});
    quests.addQuest({"q_faction_4","Фракция: Тёмный контракт",QuestType::Faction,{{"Сорвать поставку гарнизона",false}},false,-2,{{Faction::Bandits,4},{Faction::Guild,-1}}});
    quests.addQuest({"q_random_1","Случайный: Засада на тракте",QuestType::Random,{{"Отразить налёт",false}},false,0,{{Faction::Guild,1}}});
    quests.addQuest({"q_random_2","Случайный: Обвал в шахте",QuestType::Random,{{"Спасти шахтёров",false}},false,1,{{Faction::Guild,2}}});
}

} // namespace rpg
