#include "UseItem.h"

#include "Buff.h"
#include "Character.h"
#include "CharacterSpells.h"
#include "CooldownControl.h"
#include "Proc.h"

UseItem::UseItem(Character* pchar, const QString& name, const QString& icon, const int cooldown, Buff* buff, Proc* proc) :
    Spell(name, icon, pchar, new CooldownControl(pchar, cooldown), RestrictedByGcd::No, ResourceType::Rage, 0), buff(buff), proc(proc) {
    this->enabled = false;
    pchar->get_spells()->add_spell_group({this}, RELINK_ROTATION);
    buff->enable_buff();

    if (proc != nullptr)
        proc->enable_proc();
}

UseItem::~UseItem() {
    buff->disable_buff();

    if (proc != nullptr)
        proc->disable_proc();

    pchar->get_spells()->remove_spell(this);

    delete buff;
    delete proc;
    delete cooldown;
}

void UseItem::spell_effect() {
    cooldown->add_spell_cd_event();

    buff->apply_buff();
}
