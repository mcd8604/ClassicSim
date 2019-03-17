#include "Test.h"

#include <cassert>
#include <QDebug>

#include "Character.h"
#include "CharacterStats.h"
#include "CombatRoll.h"
#include "Druid.h"
#include "Dwarf.h"
#include "EncounterEnd.h"
#include "EncounterStart.h"
#include "Equipment.h"
#include "EquipmentDb.h"
#include "Faction.h"
#include "Gnome.h"
#include "Human.h"
#include "Hunter.h"
#include "ItemNamespace.h"
#include "Mage.h"
#include "NightElf.h"
#include "Orc.h"
#include "Paladin.h"
#include "Priest.h"
#include "Random.h"
#include "Rogue.h"
#include "Shaman.h"
#include "SimSettings.h"
#include "Talents.h"
#include "Tauren.h"
#include "TestAttackTables.h"
#include "TestCharacterStats.h"
#include "TestConditionVariableBuiltin.h"
#include "TestFelstrikerProc.h"
#include "TestHunter.h"
#include "TestMechanics.h"
#include "TestRogue.h"
#include "TestWarrior.h"
#include "Troll.h"
#include "Undead.h"
#include "Warlock.h"
#include "Warrior.h"
#include "WarriorSpells.h"
#include "Weapon.h"

Test::Test() :
    equipment_db(new EquipmentDb())
{}

Test::~Test() {
    delete equipment_db;
}

void Test::test_all() {
    qDebug() << "test_character_creation";
    test_character_creation();
    qDebug() << "test_equipment_creation";
    test_equipment_creation();
    qDebug() << "test_combat_roll_creation";
    test_combat_roll_creation();
    qDebug() << "test_random";
    test_random();
    qDebug() << "test_combat_roll_melee_hit_result";
    test_combat_roll_melee_hit_result();
    qDebug() << "test_queue";
    test_queue();

    TestMechanics().test_all();
    TestAttackTables().test_all();
    TestCharacterStats().test_all();
    TestFelstrikerProc(equipment_db).test_all();

    qDebug() << "test_warrior";
    TestWarrior().test_all();

    qDebug() << "test_rogue";
    TestRogue().test_all();

    qDebug() << "test_hunter";
    TestHunter().test_all();

    TestConditionVariableBuiltin().test_all();
}

void Test::test_queue() {
    Race* race = new Orc();
    auto* sim_settings = new SimSettings();
    auto* pchar = new Warrior(race, equipment_db, sim_settings);
    pchar->get_equipment()->set_mainhand(19103);
    pchar->get_equipment()->set_offhand(17075);
    assert(pchar->get_equipment()->get_mainhand()->get_name() == "Frostbite");
    assert(pchar->get_equipment()->get_offhand()->get_name() == "Vis'kag the Bloodletter");
    pchar->set_clvl(60);
    pchar->prepare_set_of_combat_iterations();
    auto* start_event = new EncounterStart(pchar);
    auto* end_event = new EncounterEnd(pchar->get_engine(), pchar, 300);

    pchar->get_engine()->add_event(end_event);
    pchar->get_engine()->add_event(start_event);
    pchar->get_engine()->run();
    pchar->get_engine()->dump();

    delete pchar;
    delete race;
    delete sim_settings;
}

void Test::test_random() {
    auto* random = new Random(0, 9999);

    for (int i = 0; i < 10; ++i) {
        qDebug() << "Random rolling " << random->get_roll();
    }

    delete random;
}

void Test::test_combat_roll_melee_hit_result() {
    Race* race = new Orc();
    auto* sim_settings = new SimSettings();
    auto* pchar = new Warrior(race, equipment_db, sim_settings);
    pchar->get_equipment()->set_mainhand(19103);
    assert(pchar->get_equipment()->get_mainhand()->get_name() == "Frostbite");

    for (int i = 0; i < 30; ++i) {
        pchar->get_combat_roll()->get_melee_hit_result(300, pchar->get_stats()->get_mh_crit_chance());
    }

    delete sim_settings;
    delete race;
    delete pchar;
}

void Test::test_combat_roll_creation() {
    auto* race = new Orc();
    auto* sim_settings = new SimSettings();
    auto* pchar = new Warrior(race, equipment_db, sim_settings);
    pchar->get_equipment()->set_mainhand(19103);
    assert(pchar->get_equipment()->get_mainhand()->get_name() == "Frostbite");

    pchar->get_combat_roll()->get_melee_white_table(300);
    pchar->get_combat_roll()->get_melee_white_table(300);
    pchar->get_combat_roll()->get_melee_white_table(315);
    pchar->get_combat_roll()->get_melee_white_table(315);
    pchar->get_combat_roll()->get_melee_white_table(300);

    delete sim_settings;
    delete race;
    delete pchar;
}

void Test::test_equipment_creation() {
    auto* equipment = new Equipment(equipment_db, nullptr);
    equipment->set_mainhand(19103);
    Weapon* mh = equipment->get_mainhand();

    assert(mh != nullptr);
    assert(mh->get_name() == "Frostbite");
    assert(mh->get_weapon_type() == WeaponTypes::AXE);
    assert(mh->get_min_dmg() == 80);
    assert(mh->get_max_dmg() == 150);
    assert(TestUtils::almost_equal(2.7, mh->get_base_weapon_speed()));

    equipment->set_ranged(17069);
    Weapon* ranged = equipment->get_ranged();
    assert(ranged != nullptr);
    assert(ranged->get_name() == "Striker's Mark");
    assert(ranged->get_weapon_type() == WeaponTypes::BOW);
    assert(ranged->get_min_dmg() == 69);
    assert(ranged->get_max_dmg() == 129);
    assert(TestUtils::almost_equal(2.5, ranged->get_base_weapon_speed()));

    delete equipment;
}

void Test::test_character_creation() {
    Race* race = new Human();
    assert(race->get_name() == "Human");
    assert(race->get_axe_bonus() == 0);
    assert(race->get_mace_bonus() == 5);
    assert(race->get_sword_bonus() == 5);
    assert(race->get_bow_bonus() == 0);
    assert(race->get_gun_bonus() == 0);
    delete race;

    race = new Dwarf();
    assert(race->get_name() == "Dwarf");
    assert(race->get_axe_bonus() == 0);
    assert(race->get_mace_bonus() == 0);
    assert(race->get_sword_bonus() == 0);
    assert(race->get_bow_bonus() == 0);
    assert(race->get_gun_bonus() == 5);
    delete race;

    race = new NightElf();
    assert(race->get_name() == "Night Elf");
    assert(race->get_axe_bonus() == 0);
    assert(race->get_mace_bonus() == 0);
    assert(race->get_sword_bonus() == 0);
    assert(race->get_bow_bonus() == 0);
    assert(race->get_gun_bonus() == 0);
    delete race;

    race = new Gnome();
    assert(race->get_name() == "Gnome");
    assert(race->get_axe_bonus() == 0);
    assert(race->get_mace_bonus() == 0);
    assert(race->get_sword_bonus() == 0);
    assert(race->get_bow_bonus() == 0);
    assert(race->get_gun_bonus() == 0);
    delete race;

    race = new Orc();
    assert(race->get_name() == "Orc");
    assert(race->get_axe_bonus() == 5);
    assert(race->get_mace_bonus() == 0);
    assert(race->get_sword_bonus() == 0);
    assert(race->get_bow_bonus() == 0);
    assert(race->get_gun_bonus() == 0);
    delete race;

    race = new Undead();
    assert(race->get_name() == "Undead");
    assert(race->get_axe_bonus() == 0);
    assert(race->get_mace_bonus() == 0);
    assert(race->get_sword_bonus() == 0);
    assert(race->get_bow_bonus() == 0);
    assert(race->get_gun_bonus() == 0);
    delete race;

    race = new Tauren();
    assert(race->get_name() == "Tauren");
    assert(race->get_axe_bonus() == 0);
    assert(race->get_mace_bonus() == 0);
    assert(race->get_sword_bonus() == 0);
    assert(race->get_bow_bonus() == 0);
    assert(race->get_gun_bonus() == 0);
    delete race;

    race = new Troll();
    assert(race->get_name() == "Troll");
    assert(race->get_axe_bonus() == 0);
    assert(race->get_mace_bonus() == 0);
    assert(race->get_sword_bonus() == 0);
    assert(race->get_bow_bonus() == 5);
    assert(race->get_gun_bonus() == 0);

    auto* priest = new Priest(race, equipment_db, nullptr);
    delete priest;

    auto* rogue = new Rogue(race, equipment_db, nullptr);
    delete rogue;

    auto* mage = new Mage(race, equipment_db, nullptr);
    delete mage;

    auto* druid = new Druid(race, equipment_db, nullptr);
    delete druid;

    auto* hunter = new Hunter(race, equipment_db, nullptr);
    delete hunter;

    auto* warlock = new Warlock(race, equipment_db, nullptr);
    delete warlock;

    auto* shaman = new Shaman(race, equipment_db, nullptr);
    delete shaman;

    auto* paladin = new Paladin(race, equipment_db, nullptr);
    paladin->set_clvl(60);
    assert(paladin->get_clvl() == 60);
    delete paladin;

    delete race;
}
