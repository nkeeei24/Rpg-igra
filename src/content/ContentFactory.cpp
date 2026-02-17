#include "content/ContentFactory.h"

namespace rpg {

void ContentFactory::populateWorld(World& world) {
    auto& loc = world.locationsMutable();
    loc["eldenport"] = {"eldenport", "Eldenport (City)", LocationType::City, {"silverwood", "ruins"}, {"mayor", "merchant", "captain"}, {}};
    loc["silverwood"] = {"silverwood", "Silverwood Forest", LocationType::Forest, {"eldenport", "caves"}, {"hunter", "druid"}, {"wolf", "bandit", "spider"}};
    loc["ruins"] = {"ruins", "Sunken Ruins", LocationType::Ruins, {"eldenport", "dungeon"}, {"scholar", "smuggler"}, {"skeleton", "cultist"}};
    loc["caves"] = {"caves", "Ashen Caves", LocationType::Caves, {"silverwood", "dungeon"}, {"priest", "miner"}, {"slime", "goblin", "ogre"}};
    loc["dungeon"] = {"dungeon", "Citadel Depths", LocationType::Dungeon, {"ruins", "caves"}, {"oracle", "bandit_lord"}, {"wraith", "knight", "boss"}};

    auto& n = world.npcs();
    n["mayor"]={"mayor","Mayor Thorne",Faction::Nobles,0,{"q_main_1","q_side_1"}};
    n["merchant"]={"merchant","Mira the Merchant",Faction::Guild,0,{"q_side_2"}};
    n["captain"]={"captain","Captain Rusk",Faction::Guild,0,{"q_faction_1"}};
    n["hunter"]={"hunter","Old Hunter Bram",Faction::Guild,0,{"q_side_3"}};
    n["druid"]={"druid","Druid Elowen",Faction::Church,0,{"q_side_4"}};
    n["scholar"]={"scholar","Scholar Venn",Faction::Nobles,0,{"q_main_2"}};
    n["smuggler"]={"smuggler","Shade the Smuggler",Faction::Bandits,0,{"q_faction_2"}};
    n["priest"]={"priest","Father Malric",Faction::Church,0,{"q_side_5"}};
    n["miner"]={"miner","Miner Dagg",Faction::Guild,0,{"q_random_1"}};
    n["oracle"]={"oracle","Blind Oracle",Faction::Church,0,{"q_main_3"}};

    auto addEnemy=[&](std::string id,std::string name,int hp,int ac,int atk,int d,int spd,int xp,int gold){
        world.enemies()[id]={id,name,{hp,hp,ac,atk,d,spd,8},xp,gold,{"iron_sword","health_potion"}};
    };
    addEnemy("wolf","Dire Wolf",42,11,3,6,12,25,10);
    addEnemy("bandit","Highway Bandit",50,12,4,8,11,30,12);
    addEnemy("spider","Venom Spider",38,12,5,6,13,28,14);
    addEnemy("skeleton","Ancient Skeleton",55,13,4,8,8,35,16);
    addEnemy("cultist","Ash Cultist",48,12,5,10,10,38,18);
    addEnemy("slime","Acid Slime",60,10,4,6,7,30,14);
    addEnemy("goblin","Cave Goblin",45,11,5,6,12,32,15);
    addEnemy("ogre","Cave Ogre",95,12,6,10,6,50,22);
    addEnemy("wraith","Night Wraith",70,14,6,10,14,60,25);
    addEnemy("knight","Fallen Knight",90,15,7,12,10,75,30);
    addEnemy("boss","Malgor the Tyrant",170,16,9,12,12,250,150);

    auto addItem=[&](std::string id,std::string name,ItemType t,Rarity r,int val,double w,int p,bool c=false){ world.items()[id]={id,name,t,r,val,w,p,c};};
    addItem("iron_sword","Iron Sword",ItemType::Weapon,Rarity::Common,20,4,2);
    addItem("steel_sword","Steel Sword",ItemType::Weapon,Rarity::Uncommon,40,4.5,4);
    addItem("oak_staff","Oak Staff",ItemType::Weapon,Rarity::Common,18,3,3);
    addItem("moon_dagger","Moon Dagger",ItemType::Weapon,Rarity::Rare,65,1.5,5);
    addItem("warhammer","Warhammer",ItemType::Weapon,Rarity::Rare,70,6,6);
    addItem("cloth_armor","Cloth Armor",ItemType::Armor,Rarity::Common,15,3,1);
    addItem("chainmail","Chainmail",ItemType::Armor,Rarity::Uncommon,45,8,3);
    addItem("knight_plate","Knight Plate",ItemType::Armor,Rarity::Epic,120,12,5);
    addItem("ring_focus","Ring of Focus",ItemType::Ring,Rarity::Rare,80,0.1,2);
    addItem("ring_guard","Ring of Guard",ItemType::Ring,Rarity::Uncommon,55,0.1,2);
    addItem("health_potion","Health Potion",ItemType::Potion,Rarity::Common,12,0.5,30,true);
    addItem("mana_tonic","Mana Tonic",ItemType::Potion,Rarity::Uncommon,18,0.5,0,true);
    addItem("elixir","Elixir of Giants",ItemType::Potion,Rarity::Rare,60,0.8,0,true);
    addItem("scroll_fire","Scroll of Fire",ItemType::Scroll,Rarity::Uncommon,30,0.2,10,true);
    addItem("scroll_ward","Scroll of Ward",ItemType::Scroll,Rarity::Rare,45,0.2,0,true);
    addItem("bandage","Field Bandage",ItemType::Potion,Rarity::Common,8,0.3,12,true);
    addItem("lockpick","Lockpick Set",ItemType::Quest,Rarity::Common,10,0.2,0);
    addItem("sun_shard","Sun Shard",ItemType::Quest,Rarity::Epic,0,0.4,0);
    addItem("oracle_seal","Oracle Seal",ItemType::Quest,Rarity::Legendary,0,0.1,0);
    addItem("tyrant_crown","Tyrant Crown",ItemType::Quest,Rarity::Legendary,0,1.0,0);
}

void ContentFactory::populateQuests(QuestSystem& quests) {
    quests.addQuest({"q_main_1","Main: Shadows over Eldenport",QuestType::Main,{{"Speak to Mayor Thorne",false},{"Defeat bandits in Silverwood",false}},false,1,{{Faction::Nobles,2},{Faction::Guild,1}}});
    quests.addQuest({"q_main_2","Main: Secrets of the Ruins",QuestType::Main,{{"Find Scholar Venn",false},{"Retrieve the Sun Shard",false}},false,1,{{Faction::Nobles,1}}});
    quests.addQuest({"q_main_3","Main: Crown of the Tyrant",QuestType::Main,{{"Consult the Oracle",false},{"Defeat Malgor in Citadel Depths",false}},false,2,{{Faction::Church,2}}});
    quests.addQuest({"q_side_1","Side: Missing Caravan",QuestType::Side,{{"Track caravan route",false},{"Rescue survivors",false}},false,1,{{Faction::Guild,1}}});
    quests.addQuest({"q_side_2","Side: Supply Trouble",QuestType::Side,{{"Gather herbs",false},{"Return to merchant",false}},false,0,{{Faction::Guild,1}}});
    quests.addQuest({"q_side_3","Side: Predator Hunt",QuestType::Side,{{"Slay a dire wolf",false}},false,0,{{Faction::Guild,1}}});
    quests.addQuest({"q_side_4","Side: Grove Cleansing",QuestType::Side,{{"Burn corrupted roots",false}},false,1,{{Faction::Church,1}}});
    quests.addQuest({"q_side_5","Side: Last Rites",QuestType::Side,{{"Recover lost relic",false}},false,1,{{Faction::Church,2}}});
    quests.addQuest({"q_faction_1","Faction: Guild Oath",QuestType::Faction,{{"Assist city guard patrol",false}},false,0,{{Faction::Guild,3}}});
    quests.addQuest({"q_faction_2","Faction: Bandit Bargain",QuestType::Faction,{{"Smuggle crate through ruins",false}},false,-1,{{Faction::Bandits,3},{Faction::Nobles,-1}}});
    quests.addQuest({"q_random_1","Random: Cave Collapse",QuestType::Random,{{"Save trapped miners",false}},false,1,{{Faction::Guild,2}}});
}

} // namespace rpg
