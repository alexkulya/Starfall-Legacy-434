#include "bastion_of_twilight.h"

enum Yells
{
    // Halfus
   SAY_AGGRO            = 0,
   SAY_KILL             = 1,
   SAY_DIE              = 2,
   SAY_DRAGON_1         = 3,
   SAY_DRAGON_2         = 4,
   SAY_DRAGON_3         = 5,
   SAY_BIND_ANNOUNCE    = 6,
   SAY_ROAR             = 7,

   // Cho'Gall
   SAY_INTRO            = 0,
   SAY_HALFUS_DIED      = 1,
};

enum Spells
{
    // Halfus Wyrmbreaker
    SPELL_FURIOUS_ROAR = 83710,

    SPELL_MALEVOLENT_STRIKES = 39171,
    SPELL_FRENZIED_ASSAULT = 83693,

    SPELL_SHADOW_WRAPPED = 83952,
    SPELL_SHADOW_NOVA = 83703,

    SPELL_BERSERK = 26662,
    SPELL_BIND_WILL = 83432,

    // Proto-Behemoth 
    SPELL_ROOT                               = 42716,
    SPELL_FIREBALL                           = 86058,
    SPELL_FIREBALL_TD                        = 83862,
    SPELL_SCORCHING_BREATH                   = 83707,
    SPELL_FIREBALL_BARRAGE                   = 83706,
    SPELL_FIREBALL_BARRAGE_DAMAGE            = 83721,
    SPELL_FIREBALL_BARRAGE_DAMAGE_TD         = 83733,

    SPELL_DANCING_FLAMES                     = 84106,
    SPELL_SUPERHEATED_BREATH                 = 83956,

    // Dragon debuffs on Halfus
    SPELL_NETHER_BLINDNESS                   = 83611,
    SPELL_CYCLONE_WINDS                      = 84092,
    SPELL_ATROPHIC_POISON                    = 83609,
    SPELL_TIME_DILATION                      = 83601,
    SPELL_STONE_TOUCH_NORMAL                 = 83603,
    SPELL_STONE_TOUCH_HEROIC                 = 84593,
    SPELL_DRAGONS_VENGEANCE                  = 87683,

    // Dragons
    SPELL_UNRESPONSIVE_DRAGON                = 86003,
    SPELL_UNRESPONSIVE_WHELP                 = 86022,
    SPELL_FREE_DRAGON                        = 83590,

    // Spike
    SPELL_CHAIN                              = 83487,
};

enum Events
{
    // Halfus
    EVENT_SHADOW_NOVA = 1,
    EVENT_FURIOUS_ROAR,
    EVENT_FURIOUS_ROAR_CAST,
    EVENT_BERSERK,
    EVENT_TALK_ROAR,
    EVENT_APPLY_IMMUNITY,

    // Behemoth
    EVENT_MOVE_UP,
    EVENT_ROOT,
    EVENT_FIREBALL,
    EVENT_SCORCHING_BREATH,
    EVENT_FIREBALL_BARRAGE,

    EVENT_ATTACK,
};

enum Phases
{
    PHASE_1 = 1,
    PHASE_2,
};

enum Actions
{
    ACTION_INTRO = 1,
    ACTION_ORPHAN_KILLED,
};

Position const NetherScionSetup[] =
{
    { -280.118f, -659.385f, 888.093f, 1.67552f }, // Nether Scion
    { -274.891f, -656.835f, 888.182f, 4.95674f },
    { -284.868f, -661.151f, 888.183f, 1.71042f },
};

Position const SlateDragonSetup[] =
{
    { -279.111f, -680.594f, 888.086f, 3.10669f }, // Slate Dragon
    { -273.922f, -675.948f, 888.182f, 6.19592f },
    { -273.010f, -684.694f, 888.184f, 3.15905f },
    { -280.724f, -674.460f, 888.183f, 0.08726f },
    { -277.955f, -687.788f, 888.185f, 3.15905f },
    { -283.142f, -685.325f, 888.184f, 3.15905f },
};

Position const StormRiderSetup[] =
{
    { -318.705f, -722.688f, 888.086f, 3.19395f }, // Storm Rider
    { -321.974f, -718.424f, 888.172f, 0.38397f },
    { -320.158f, -727.995f, 888.172f, 3.24631f },
    { -314.986f, -717.396f, 888.171f, 0.26179f },
};

Position const OrphanedEmeraldWhelpSetup[] =
{
    { -342.002f, -726.528f, 890.083f, 1.16937f },
    { -340.75f,  -722.476f, 890.084f, 1.16937f },
    { -347.08f,  -728.128f, 890.084f, 1.11701f },
    { -346.434f, -724.049f, 890.083f, 1.09956f },
    { -344.839f, -720.163f, 890.084f, 1.0821f  },
    { -350.571f, -725.417f, 890.083f, 1.06465f },
    { -350.564f, -721.663f, 890.084f, 1.01229f },
    { -347.578f, -717.302f, 890.084f, 1.0472f  },
};

Position const TimeWardenSetup[] =
{
    { -353.179f, -700.667f, 888.104f, 5.53269f }, // Time Warden
    { -358.964f, -702.148f, 888.171f, 2.30383f },
    { -353.764f, -706.361f, 888.171f, 2.30383f },
    { -352.804f, -695.064f, 888.183f, 5.35816f },
    { -348.538f, -700.247f, 888.183f, 5.53269f },
};

Position const ProtoBehemothSetup[] = { -265.891f, -740.023f, 907.363f, 2.33874f };

class DragonDistanceCheck
{
public:
    DragonDistanceCheck(Unit* caster) : caster(caster) { }

    bool operator()(WorldObject* object)
    {
        return (object->GetDistance2d(caster) >= 5.0f);
    }
private:
    Unit* caster;
};

class at_bot_intro_1 : public AreaTriggerScript
{
    public:
        at_bot_intro_1() : AreaTriggerScript("at_bot_intro_1") { }

        bool OnTrigger(Player* player, AreaTriggerEntry const* /*areaTrigger*/)
        {
            if (InstanceScript* instance = player->GetInstanceScript())
                if (Creature* halfus = ObjectAccessor::GetCreature(*player, instance->GetData64(DATA_HALFUS_WYRMBREAKER)))
                    halfus->AI()->DoAction(ACTION_INTRO);
            return true;
        }
};

class boss_halfus_wyrmbreaker : public CreatureScript
{
    public:
        boss_halfus_wyrmbreaker() : CreatureScript("boss_halfus_wyrmbreaker") { }
            
        struct boss_halfus_wyrmbreakerAI : public BossAI
        {
            boss_halfus_wyrmbreakerAI(Creature* creature) : BossAI(creature, DATA_HALFUS_WYRMBREAKER)
            {
                _roarCasts = 0;
                _orphanKilled = 0;
                _combinationPicked = 0;
                _introDone = false;
            }

            uint8 _roarCasts;
            uint8 _combinationPicked;
            uint8 _orphanKilled;
            bool _introDone;

            void EnterCombat(Unit* /*who*/)
            {
                _EnterCombat();
                Talk(SAY_AGGRO);
                instance->SendEncounterUnit(ENCOUNTER_FRAME_ENGAGE, me);
                events.ScheduleEvent(EVENT_BERSERK, 360000);
                if (me->HasAura(SPELL_SHADOW_WRAPPED))
                    events.ScheduleEvent(EVENT_SHADOW_NOVA, urand(7000, 10000));
            }

            void Reset()
            {
                _Reset();
                _combinationPicked = instance->GetData(DATA_DRAGONS_PICKED);
                InitializeDragons();
                me->ApplySpellImmune(0, IMMUNITY_MECHANIC, MECHANIC_INTERRUPT, true);
                me->ApplySpellImmune(0, IMMUNITY_EFFECT, SPELL_EFFECT_INTERRUPT_CAST, true);
            }

            void JustRespawned()
            {
                InitializeDragons();
                instance->SetBossState(DATA_HALFUS_WYRMBREAKER, NOT_STARTED);
                if (GameObject* cage = ObjectAccessor::GetGameObject(*me, instance->GetData64(DATA_CAGE)))
                    cage->RemoveFlag(GAMEOBJECT_FLAGS, GO_FLAG_NOT_SELECTABLE | GO_FLAG_IN_USE);
            }

            void EnterEvadeMode()
            {
                _EnterEvadeMode();
                instance->SendEncounterUnit(ENCOUNTER_FRAME_DISENGAGE, me);
                me->GetMotionMaster()->MoveTargetedHome();
                summons.DespawnAll();
                events.Reset();

                _roarCasts = 0;
                _orphanKilled = 0;

                me->ApplySpellImmune(0, IMMUNITY_MECHANIC, MECHANIC_INTERRUPT, true);
                me->ApplySpellImmune(0, IMMUNITY_EFFECT, SPELL_EFFECT_INTERRUPT_CAST, true);

                if (GameObject* cage = ObjectAccessor::GetGameObject(*me, instance->GetData64(DATA_CAGE)))
                {
                    cage->SetGoState(GO_STATE_READY);
                    cage->SetFlag(GAMEOBJECT_FLAGS, GO_FLAG_NOT_SELECTABLE);
                }

                _DespawnAtEvade();
            }

            void SpellHitTarget(Unit* target, SpellInfo const* spell)
            {
                if (spell->Id == SPELL_BIND_WILL)
                    Talk(SAY_BIND_ANNOUNCE, target->GetGUID());
            }

            void DamageTaken(Unit* target, uint32& damage)
            {
                if (me->HealthBelowPct(50) && (!(events.IsInPhase(PHASE_2))))
                {
                    events.SetPhase(PHASE_2);
                    events.ScheduleEvent(EVENT_FURIOUS_ROAR, 1000);
                }
            }

            void DoAction(int32 action)
            {
                switch (action)
                {
                    case ACTION_INTRO:
                        if (!_introDone)
                            if (Creature* chogall = ObjectAccessor::GetCreature(*me, instance->GetData64(DATA_CHOGALL_HALFUS_INTRO)))
                            {
                                chogall->AI()->TalkToMap(0);
                                _introDone = true;
                            }
                        break;
                    case ACTION_ORPHAN_KILLED:
                        _orphanKilled++;
                        if (_orphanKilled >= 8)
                            me->AddAura(SPELL_DRAGONS_VENGEANCE, me);
                        break;
                }
            }

            void JustDied(Unit* killer)
            {
                _JustDied();
                instance->SendEncounterUnit(ENCOUNTER_FRAME_DISENGAGE, me);
                summons.DespawnAll();
                if (Creature* chogall = ObjectAccessor::GetCreature(*me, instance->GetData64(DATA_CHOGALL_HALFUS_INTRO)))
                    chogall->AI()->TalkToMap(SAY_HALFUS_DIED);
            }

            void KilledUnit(Unit* killed)
            {
                if (killed->GetTypeId() == TYPEID_PLAYER)
                    Talk(SAY_KILL);
            }

            void JustSummoned(Creature* summon)
            {
                switch (summon->GetEntry())
                {
                    case NPC_SPIKE:
                        summon->CastWithDelay(100, summon, SPELL_CHAIN);
                        break;
                    case NPC_SLATE_DRAGON:
                        switch (_combinationPicked)
                        {
                            case 4:
                            case 7:
                            case 8:
                            case 9:
                                summon->AddAura(SPELL_UNRESPONSIVE_DRAGON, summon);
                                break;
                            default:
                                me->AddAura(SPELL_MALEVOLENT_STRIKES, me);
                                summon->SetFlag(UNIT_NPC_FLAGS, UNIT_NPC_FLAG_GOSSIP);
                                break;
                        }
                        break;
                    case NPC_NETHER_SCION:
                        switch (_combinationPicked)
                        {
                            case 1:
                            case 3:
                            case 6:
                            case 8:
                                summon->AddAura(SPELL_UNRESPONSIVE_DRAGON, summon);
                            break;
                            default:
                                me->AddAura(SPELL_FRENZIED_ASSAULT, me);
                                summon->SetFlag(UNIT_NPC_FLAGS, UNIT_NPC_FLAG_GOSSIP);
                                break;
                        }
                        break;
                    case NPC_STORM_RIDER:
                        switch (_combinationPicked)
                        {
                            case 2:
                            case 6:
                            case 7:
                            case 10:
                                summon->AddAura(SPELL_UNRESPONSIVE_DRAGON, summon);
                                break;
                            default:
                                me->AddAura(SPELL_SHADOW_WRAPPED, me);
                                summon->SetFlag(UNIT_NPC_FLAGS, UNIT_NPC_FLAG_GOSSIP);
                                break;
                        }
                        break;
                    case NPC_ORPHANED_EMERALD_WHELP:
                        switch (_combinationPicked)
                        {
                            case 2:
                            case 3:
                            case 4:
                            case 5:
                                summon->AddAura(SPELL_UNRESPONSIVE_DRAGON, summon);
                                summon->RemoveFlag(UNIT_NPC_FLAGS, UNIT_NPC_FLAG_GOSSIP);
                                if (GameObject* cage = ObjectAccessor::GetGameObject(*me, instance->GetData64(DATA_CAGE)))
                                    cage->SetFlag(GAMEOBJECT_FLAGS, GO_FLAG_NOT_SELECTABLE);
                                break;
                            default:
                                summon->SetFlag(UNIT_NPC_FLAGS, UNIT_NPC_FLAG_GOSSIP);
                                break;
                        }
                        break;
                    case NPC_TIME_WARDEN:
                        switch (_combinationPicked)
                        {
                            case 1:
                            case 5:
                            case 9:
                            case 10:
                                summon->AddAura(SPELL_UNRESPONSIVE_DRAGON, summon);
                                break;
                            default:
                                summon->SetFlag(UNIT_NPC_FLAGS, UNIT_NPC_FLAG_GOSSIP);
                                break;
                        }
                        break;
                    default:
                        break;
                }
                summons.Summon(summon);
            }

            void InitializeDragons()
            {
                // Slate Dragon Setup
                me->SummonCreature(NPC_SLATE_DRAGON, SlateDragonSetup[0], TEMPSUMMON_MANUAL_DESPAWN);
                for (uint8 i = 1; i < 7; i++)
                    me->SummonCreature(NPC_SPIKE, SlateDragonSetup[i], TEMPSUMMON_MANUAL_DESPAWN);

                // Nether Scion Setup
                me->SummonCreature(NPC_NETHER_SCION, NetherScionSetup[0], TEMPSUMMON_MANUAL_DESPAWN);
                for (uint8 i = 1; i < 3; i++)
                    me->SummonCreature(NPC_SPIKE, NetherScionSetup[i], TEMPSUMMON_MANUAL_DESPAWN);

                // Storm Rider Setup
                me->SummonCreature(NPC_STORM_RIDER, StormRiderSetup[0], TEMPSUMMON_MANUAL_DESPAWN);
                for (uint8 i = 1; i < 4; i++)
                    me->SummonCreature(NPC_SPIKE, StormRiderSetup[i], TEMPSUMMON_MANUAL_DESPAWN);

                // Time Warden Setup
                me->SummonCreature(NPC_TIME_WARDEN, TimeWardenSetup[0], TEMPSUMMON_MANUAL_DESPAWN);
                for (uint8 i = 1; i < 5; i++)
                    me->SummonCreature(NPC_SPIKE, TimeWardenSetup[i], TEMPSUMMON_MANUAL_DESPAWN);

                // Orphaned Emerald Whelp Setup
                for (uint8 i = 0; i < 8; i++)
                    me->SummonCreature(NPC_ORPHANED_EMERALD_WHELP, OrphanedEmeraldWhelpSetup[i], TEMPSUMMON_MANUAL_DESPAWN);

                me->SummonCreature(NPC_PROTO_BEHEMOTH, ProtoBehemothSetup[0], TEMPSUMMON_MANUAL_DESPAWN);
            }

            /*
            void SetupDragons(uint8 combinationNumber)
            {
                switch (combinationNumber)
                {
                case 1:  // Slate, Storm, Whelps.
                    me->AddAura(SPELL_SHADOW_WRAPPED, me);
                    me->AddAura(SPELL_MALEVOLENT_STRIKES, me);
                    me->AddAura(SPELL_SUPERHEATED_BREATH, protoDrake);
                    me->AddAura(SPELL_UNRESPONSIVE_DRAGON, netherScion);
                    me->AddAura(SPELL_UNRESPONSIVE_DRAGON, timeRider);
                    break;
                case 2:  // Slate, Nether, Time.
                    CastUnresponsiveWhelps();
                    me->AddAura(SPELL_FRENZIED_ASSAULT, me);
                    me->AddAura(SPELL_MALEVOLENT_STRIKES, me);
                    me->AddAura(SPELL_DANCING_FLAMES, protoDrake);
                    me->AddAura(SPELL_UNRESPONSIVE_DRAGON, stormRider);
                    break;
                case 3:  // Slate, Storm, Time.
                    CastUnresponsiveWhelps();
                    me->AddAura(SPELL_MALEVOLENT_STRIKES, me);
                    me->AddAura(SPELL_SHADOW_WRAPPED, me);
                    me->AddAura(SPELL_DANCING_FLAMES, protoDrake);
                    me->AddAura(SPELL_UNRESPONSIVE_DRAGON, netherScion);
                    break;
                case 4:  // Storm, Nether, Time.
                    CastUnresponsiveWhelps();
                    me->AddAura(SPELL_FRENZIED_ASSAULT, me);
                    me->AddAura(SPELL_SHADOW_WRAPPED, me);
                    me->AddAura(SPELL_DANCING_FLAMES, protoDrake);
                    me->AddAura(SPELL_UNRESPONSIVE_DRAGON, slateDragon);
                    break;
                case 5:  // Slate, Storm, Nether.
                    CastUnresponsiveWhelps();
                    me->AddAura(SPELL_FRENZIED_ASSAULT, me);
                    me->AddAura(SPELL_MALEVOLENT_STRIKES, me);
                    me->AddAura(SPELL_UNRESPONSIVE_DRAGON, timeRider);
                    me->AddAura(SPELL_SHADOW_WRAPPED, me);
                    break;
                case 6:  // Slate, Whelps, Time.
                    me->AddAura(SPELL_MALEVOLENT_STRIKES, me);
                    me->AddAura(SPELL_SUPERHEATED_BREATH, protoDrake);
                    me->AddAura(SPELL_DANCING_FLAMES, protoDrake);
                    me->AddAura(SPELL_UNRESPONSIVE_DRAGON, netherScion);
                    me->AddAura(SPELL_UNRESPONSIVE_DRAGON, stormRider);
                    break;
                case 7:  // Whelps, Nether, Time.
                    me->AddAura(SPELL_FRENZIED_ASSAULT, me);
                    me->AddAura(SPELL_SUPERHEATED_BREATH, protoDrake);
                    me->AddAura(SPELL_DANCING_FLAMES, protoDrake);
                    me->AddAura(SPELL_UNRESPONSIVE_DRAGON, slateDragon);
                    me->AddAura(SPELL_UNRESPONSIVE_DRAGON, stormRider);
                    break;
                case 8:  // Storm, Whelps, Time.
                    me->AddAura(SPELL_DANCING_FLAMES, protoDrake);
                    me->AddAura(SPELL_SUPERHEATED_BREATH, protoDrake);
                    me->AddAura(SPELL_UNRESPONSIVE_DRAGON, netherScion);
                    me->AddAura(SPELL_UNRESPONSIVE_DRAGON, slateDragon);
                    me->AddAura(SPELL_SHADOW_WRAPPED, me);
                    break;
                case 9:  // Storm, Whelps, Nether.
                    me->AddAura(SPELL_SUPERHEATED_BREATH, protoDrake);
                    me->AddAura(SPELL_FRENZIED_ASSAULT, me);
                    me->AddAura(SPELL_UNRESPONSIVE_DRAGON, timeRider);
                    me->AddAura(SPELL_UNRESPONSIVE_DRAGON, slateDragon);
                    me->AddAura(SPELL_SHADOW_WRAPPED, me);
                    break;
                case 10: // Slate, Whelps, Nether.
                    me->AddAura(SPELL_SUPERHEATED_BREATH, protoDrake);
                    me->AddAura(SPELL_FRENZIED_ASSAULT, me);
                    me->AddAura(SPELL_MALEVOLENT_STRIKES, me);
                    me->AddAura(SPELL_UNRESPONSIVE_DRAGON, timeRider);
                    me->AddAura(SPELL_UNRESPONSIVE_DRAGON, stormRider);
                    break;
                default:
                    break;
                }
            }
            }
            */

            void UpdateAI(uint32 diff)
            {
                if (!UpdateVictim())
                    return;

                events.Update(diff);

                while (uint32 eventId = events.ExecuteEvent())
                {
                    switch(eventId)
                    {
                        case EVENT_SHADOW_NOVA:
                            DoCastAOE(SPELL_SHADOW_NOVA);
                            me->ApplySpellImmune(0, IMMUNITY_MECHANIC, MECHANIC_INTERRUPT, false);
                            me->ApplySpellImmune(0, IMMUNITY_EFFECT, SPELL_EFFECT_INTERRUPT_CAST, false);
                            if (!me->HasAura(SPELL_CYCLONE_WINDS))
                                events.ScheduleEvent(EVENT_APPLY_IMMUNITY, 500);
                            else
                                events.ScheduleEvent(EVENT_APPLY_IMMUNITY, 2500);
                            events.ScheduleEvent(EVENT_SHADOW_NOVA, urand(10000, 17000));
                            break;
                        case EVENT_FURIOUS_ROAR:
                            if (_roarCasts < 3)
                            {
                                _roarCasts++;
                                DoCastAOE(SPELL_FURIOUS_ROAR);
                                events.ScheduleEvent(EVENT_TALK_ROAR, me->GetCurrentSpellCastTime(SPELL_FURIOUS_ROAR));
                                events.ScheduleEvent(EVENT_FURIOUS_ROAR, me->GetCurrentSpellCastTime(SPELL_FURIOUS_ROAR) + 100);
                            }
                            else
                            {
                                events.ScheduleEvent(EVENT_TALK_ROAR, me->GetCurrentSpellCastTime(SPELL_FURIOUS_ROAR));
                                events.ScheduleEvent(EVENT_FURIOUS_ROAR, 30000);
                            }
                            break;
                        case EVENT_TALK_ROAR:
                            Talk(SAY_ROAR);
                            break;
                        case EVENT_BERSERK:
                            me->AddAura(SPELL_BERSERK, me);
                            break;
                        case EVENT_APPLY_IMMUNITY:
                            me->ApplySpellImmune(0, IMMUNITY_MECHANIC, MECHANIC_INTERRUPT, true);
                            me->ApplySpellImmune(0, IMMUNITY_EFFECT, SPELL_EFFECT_INTERRUPT_CAST, true);
                            break;
                        default:
                            break;
                    }
                }

                DoMeleeAttackIfReady();
            }
        };
        CreatureAI* GetAI(Creature* creature) const
        {
            return new boss_halfus_wyrmbreakerAI(creature);
        }
};

class npc_proto_behemoth : public CreatureScript
{
    public:
        npc_proto_behemoth() : CreatureScript("npc_proto_behemoth") { }
            
        struct npc_proto_behemothAI : public ScriptedAI
        {
            npc_proto_behemothAI(Creature* creature) : ScriptedAI(creature)
            {
                instance = creature->GetInstanceScript();
                _currentCombination = 0;
            }

            InstanceScript* instance;
            EventMap events;
            uint8 _currentCombination;

            void InitializeAI()
            {
                me->SetReactState(REACT_PASSIVE);
            }

            void IsSummonedBy(Unit* /*summoner*/)
            {
                _currentCombination = instance->GetData(DATA_DRAGONS_PICKED);
                SetupBuffs();
            }

            void EnterCombat(Unit* /*who*/)
            {
                events.ScheduleEvent(EVENT_FIREBALL, 16000);
            }

            void SetupBuffs()
            {
                if (_currentCombination == 2 || _currentCombination == 3 || _currentCombination == 4 ||
                    _currentCombination == 6 || _currentCombination == 7 || _currentCombination == 8)
                    me->AddAura(SPELL_DANCING_FLAMES, me);

                if (_currentCombination == 1 || _currentCombination == 6 || _currentCombination == 7 ||
                    _currentCombination == 8 || _currentCombination == 9 || _currentCombination == 10)
                    me->AddAura(SPELL_SUPERHEATED_BREATH, me);
            }

            void UpdateAI(uint32 diff)
            {
                events.Update(diff);

                while (uint32 eventId = events.ExecuteEvent())
                {
                    switch(eventId)
                    {
                        case EVENT_MOVE_UP:
                            me->GetMotionMaster()->MovePoint(1, me->GetPositionX(), me->GetPositionY(), 925.3f);
                            break;
                        case EVENT_ROOT:
                            DoCast(me, SPELL_ROOT);
                            break;
                        case EVENT_FIREBALL:
                            if (Unit* target = SelectTarget(SELECT_TARGET_RANDOM, 0, 200.0f, true))
                            {
                                if (!me->HasAura(SPELL_TIME_DILATION))
                                    DoCast(target, SPELL_FIREBALL);
                                else
                                    DoCast(target, SPELL_FIREBALL_TD);
                            }
                            if (me->HasAura(SPELL_TIME_DILATION))
                                events.ScheduleEvent(EVENT_FIREBALL, urand(18000, 25000));
                            else
                                events.ScheduleEvent(EVENT_FIREBALL, urand(4000, 7000));
                            break;
                        case EVENT_SCORCHING_BREATH:
                            DoCast(me, SPELL_SCORCHING_BREATH);
                            if (me->GetMap()->IsHeroic())
                                events.ScheduleEvent(EVENT_SCORCHING_BREATH, urand(7000, 9000));
                            else
                                events.ScheduleEvent(EVENT_SCORCHING_BREATH, urand(9000, 12000));
                            break;
                        case EVENT_FIREBALL_BARRAGE:
                            DoCast(me, SPELL_FIREBALL_BARRAGE);
                            if (me->GetMap()->IsHeroic())
                                events.ScheduleEvent(EVENT_FIREBALL_BARRAGE, urand(26000, 30000));
                            else
                                events.ScheduleEvent(EVENT_FIREBALL_BARRAGE, urand(30000, 34000));
                            break;
                        default:
                            break;
                    }
                }
            }
        };

        CreatureAI* GetAI(Creature* creature) const
        {
            return new npc_proto_behemothAI(creature);
        }
};

class npc_halfus_dragon: public CreatureScript
{
public:
    npc_halfus_dragon () : CreatureScript("npc_halfus_dragon") { }

    struct npc_halfus_dragonAI: public ScriptedAI
    {
        npc_halfus_dragonAI (Creature* creature) : ScriptedAI(creature)
        {
            instance = creature->GetInstanceScript();
        }

        InstanceScript* instance;
        EventMap events;

        void Reset()
        {
            me->DeleteThreatList();
            me->CombatStop(true);
            me->SetReactState(REACT_PASSIVE);
        }

        void MovementInform(uint32 type, uint32 point)
        {
            switch (point)
            {
                case 1:
                    events.ScheduleEvent(EVENT_ATTACK, 1);
                    break;
            }
        }

        void EnterCombat(Unit* pWho)
        {
            Creature* Halfus = me->FindNearestCreature(BOSS_HALFUS_WYRMBREAKER, 500.0f, true);

            switch(me->GetEntry())
            {
                case NPC_SLATE_DRAGON:
                    Halfus->AI()->Talk(3);
                    break;
                case NPC_NETHER_SCION:
                    Halfus->AI()->Talk(4);
                    break;
                case NPC_STORM_RIDER:
                case NPC_TIME_WARDEN:
                    Halfus->AI()->Talk(5);
                    break;
            }

            me->SetReactState(REACT_PASSIVE);
        /*
            me->SetCanFly(true);
            me->SetDisableGravity(true);
            me->SetByteFlag(UNIT_FIELD_BYTES_1, 3, UNIT_BYTE1_FLAG_ALWAYS_STAND | UNIT_BYTE1_FLAG_HOVER);
            Position pos;
            pos.Relocate(me);
            pos.m_positionZ += 5.0f;
            me->GetMotionMaster()->MoveTakeoff(1, pos);
        */
        }

        void SpellHit(Unit* target, SpellInfo const* spell)
        {
            if (spell->Id == SPELL_FREE_DRAGON)
            {
                if (Creature* Halfus = ObjectAccessor::GetCreature(*me, instance->GetData64(DATA_HALFUS_WYRMBREAKER)))
                {
                    switch (me->GetEntry())
                    {
                        case NPC_SLATE_DRAGON:
                            if (instance->instance->IsHeroic())
                                me->AddAura(SPELL_STONE_TOUCH_HEROIC, Halfus);
                            else
                                me->AddAura(SPELL_STONE_TOUCH_NORMAL, Halfus);
                            break;
                        case NPC_NETHER_SCION:
                            me->AddAura(SPELL_NETHER_BLINDNESS, Halfus);
                            break;
                        case NPC_STORM_RIDER:
                            me->AddAura(SPELL_CYCLONE_WINDS, Halfus);
                            break;
                        case NPC_TIME_WARDEN:
                            if (Creature* behemoth = me->FindNearestCreature(NPC_PROTO_BEHEMOTH, 500.0f, true))
                            {
                                me->AddAura(SPELL_TIME_DILATION, behemoth);
                            }
                            break;
                    }

                    if (Halfus->HasAura(84030))
                        Halfus->RemoveAurasDueToSpell(84030);
                    else if (Halfus->HasAura(84591))
                        Halfus->RemoveAurasDueToSpell(84591);

                    Halfus->AI()->DoZoneInCombat(Halfus, 150.0f);
                    Halfus->CastStop();
                    Halfus->CastSpell(me, SPELL_BIND_WILL);
                    me->SetReactState(REACT_AGGRESSIVE);
                    me->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
                    me->AI()->DoZoneInCombat(me);
                }
            }

        }

        void EnterEvadeMode()
        {
            Reset();
            me->RemoveAllAuras();
            me->GetMotionMaster()->MoveTargetedHome();
        }

        void JustDied(Unit* /*killer*/)
        {
            if (Creature* Halfus = me->FindNearestCreature(BOSS_HALFUS_WYRMBREAKER, 500.0f, true))
                Halfus->AddAura(SPELL_DRAGONS_VENGEANCE, Halfus);
        }

        void UpdateAI(uint32 diff)
        {
            if (!UpdateVictim() || me->HasUnitState(UNIT_STATE_CASTING))
                return;

            if (me->HasAura(SPELL_UNRESPONSIVE_DRAGON))
                me->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_NPC_FLAG_GOSSIP);

            events.Update(diff);

            while (uint32 eventId = events.ExecuteEvent())
            {
                switch (eventId)
                {
                    case EVENT_ATTACK:
                        me->SetReactState(REACT_AGGRESSIVE);
                        break;
                }
            }

            DoMeleeAttackIfReady();
        }
    };

    bool OnGossipHello (Player* player, Creature* creature)
    {
        if (!creature->HasAura(SPELL_UNRESPONSIVE_DRAGON))
        {
            player->CastSpell(creature, SPELL_FREE_DRAGON);
            player->CLOSE_GOSSIP_MENU();
        }
        return true;
    }

    CreatureAI* GetAI (Creature* creature) const
    {
        return new npc_halfus_dragonAI(creature);
    }
};

class npc_orphaned_whelp : public CreatureScript
{
public:
    npc_orphaned_whelp() : CreatureScript("npc_orphaned_whelp") { }

    struct npc_orphaned_whelpAI : public ScriptedAI
    {
        npc_orphaned_whelpAI(Creature* creature) : ScriptedAI(creature)
        {
            instance = creature->GetInstanceScript();
        }

        InstanceScript* instance;

        void Reset()
        {
            me->DeleteThreatList();
            me->CombatStop(true);
            me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
        }

        void EnterCombat(Unit* /*who*/)
        {
            if (Creature* Halfus = me->FindNearestCreature(BOSS_HALFUS_WYRMBREAKER, 500.0f, true))
            {
                if (!me->HasAura(SPELL_UNRESPONSIVE_WHELP))
                {
                    if (Creature* behemoth = me->FindNearestCreature(NPC_PROTO_BEHEMOTH, 500.0f, true))
                    {

                        me->AddAura(SPELL_ATROPHIC_POISON, behemoth);
                        behemoth->SetAuraStack(SPELL_ATROPHIC_POISON, behemoth, 8);

                        if (!behemoth->HasAura(SPELL_SUPERHEATED_BREATH))
                            me->AddAura(SPELL_SUPERHEATED_BREATH, behemoth);

                        if (!Halfus->isInCombat())
                            Halfus->AI()->DoZoneInCombat(Halfus, 150.0f);
                        Halfus->AddAura(SPELL_BIND_WILL, me);
                        me->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
                    }
                }
            }
        }

        void EnterEvadeMode ()
        {
            Reset();
            me->RemoveAllAuras();
            me->GetMotionMaster()->MoveTargetedHome();
        }

        void JustDied(Unit* /*killer*/)
        {
            if (Creature* halfus = ObjectAccessor::GetCreature(*me, instance->GetData64(DATA_HALFUS_WYRMBREAKER)))
                halfus->AI()->DoAction(ACTION_ORPHAN_KILLED);
        }

        void UpdateAI(uint32 diff) 
        {
            DoMeleeAttackIfReady();
        }
    };
    CreatureAI* GetAI(Creature* creature) const
    {
        return new npc_orphaned_whelpAI(creature);
    }
};

class spell_bot_chain : public SpellScriptLoader
{
public:
    spell_bot_chain() : SpellScriptLoader("spell_bot_chain")
    {
    }

    class spell_bot_chain_SpellScript : public SpellScript
    {
        PrepareSpellScript(spell_bot_chain_SpellScript);

        void SetTarget(WorldObject*& target)
        {
            if (Unit* caster = GetCaster())
            {
                if (Unit* slate = caster->FindNearestCreature(NPC_SLATE_DRAGON, 5.0f, true))
                    target = slate;
                else if (Unit* scion = caster->FindNearestCreature(NPC_NETHER_SCION, 5.0f, true))
                    target = scion;
                else if (Unit* rider = caster->FindNearestCreature(NPC_STORM_RIDER, 5.0f, true))
                    target = rider;
                else if (Unit* warden = caster->FindNearestCreature(NPC_TIME_WARDEN, 5.0f, true))
                    target = warden;
            }
        }

        void Register()
        {
            OnObjectTargetSelect += SpellObjectTargetSelectFn(spell_bot_chain_SpellScript::SetTarget, EFFECT_0, TARGET_UNIT_NEARBY_ENTRY);
        }
    };

    SpellScript* GetSpellScript() const
    {
        return new spell_bot_chain_SpellScript();
    }
};

class spell_proto_fireball : public SpellScriptLoader // 86058, 83862
{
public:
    spell_proto_fireball() : SpellScriptLoader("spell_proto_fireball") { }

    class spell_proto_fireballSpellScript : public SpellScript
    {
        PrepareSpellScript(spell_proto_fireballSpellScript);

        bool Validate(SpellInfo const * spellEntry)
        {
            return true;
        }

        bool Load()
        {
            return true;
        }

        void HandleDummy(SpellEffIndex effIndex)
        {
            if (GetCaster()->HasAura(SPELL_TIME_DILATION))
            {
                if (Unit* victim = GetCaster()->getVictim())
                    GetCaster()->CastSpell(victim, SPELL_FIREBALL_BARRAGE_DAMAGE_TD, false);
            }
            else
            {
                if (Unit* victim = GetCaster()->getVictim())
                    GetCaster()->CastSpell(victim, SPELL_FIREBALL_BARRAGE_DAMAGE, false);
            }
        }

        void Register()
        {
            OnEffectHit += SpellEffectFn(spell_proto_fireballSpellScript::HandleDummy,EFFECT_0,SPELL_EFFECT_DUMMY);
        }
    };

    SpellScript *GetSpellScript() const
    {
        return new spell_proto_fireballSpellScript();
    }
};

class spell_proto_fireball_barrage : public SpellScriptLoader // 83719.
{
public:
    spell_proto_fireball_barrage() : SpellScriptLoader("spell_proto_fireball_barrage") { }

    class spell_proto_fireball_barrageSpellScript : public SpellScript
    {
        PrepareSpellScript(spell_proto_fireball_barrageSpellScript);

        void HandleDummy(SpellEffIndex effIndex)
        {
            if (Unit* caster = GetCaster())
            {
                if (Map* map = caster->GetMap())
                {
                    std::list<Player*> players = caster->GetNearestPlayersList(200.f, true);

                    for (uint8 i = 0; i < (map->Is25ManRaid() ? 17 : 7); ++i)
                    {
                        if (players.empty())
                            break;

                        std::list<Player*>::const_iterator itr = players.begin();
                        std::advance(itr, urand(0, players.size() - 1));
                        caster->CastSpell(*itr, SPELL_FIREBALL_TD, true);
                        players.remove(*itr);
                    }
                }
            }
        }        

        void Register()
        {
            OnEffectHit += SpellEffectFn(spell_proto_fireball_barrageSpellScript::HandleDummy, EFFECT_0, SPELL_EFFECT_DUMMY);
        }
    };

    SpellScript *GetSpellScript() const
    {
        return new spell_proto_fireball_barrageSpellScript();
    }
};

// 84593
class spell_halfus_stone_touch: public SpellScriptLoader
{
public:
    spell_halfus_stone_touch() : SpellScriptLoader("spell_halfus_stone_touch") { }

    class spell_halfus_stone_touch_AuraScript: public AuraScript
    {
        PrepareAuraScript(spell_halfus_stone_touch_AuraScript)

        void HandleEffectPeriodic(AuraEffect const * /*aurEff*/) 
        {
            if (GetId() == 83603)
                GetTarget()->CastSpell(GetTarget(), 84030, true);
            else if (GetId() == 84593)
                GetTarget()->CastSpell(GetTarget(), 84591, true);
        }

        void Register() 
        {
            OnEffectPeriodic += AuraEffectPeriodicFn(spell_halfus_stone_touch_AuraScript::HandleEffectPeriodic, EFFECT_0, SPELL_AURA_PERIODIC_DUMMY);
        }
    };

    AuraScript *GetAuraScript() const 
    {
        return new spell_halfus_stone_touch_AuraScript();
    }
};

class spell_bot_scorching_breath_damage : public SpellScriptLoader
{
public:
    spell_bot_scorching_breath_damage() : SpellScriptLoader("spell_bot_scorching_breath_damage") { }

    class spell_bot_scorching_breath_damage_SpellScript : public SpellScript
    {
        PrepareSpellScript(spell_bot_scorching_breath_damage_SpellScript);

        void CalculateDamage(SpellEffIndex /*effIndex*/)
        {
            uint64 damage = 0;
            if (Unit* caster = GetCaster())
                if (Aura* poison = caster->GetAura(SPELL_ATROPHIC_POISON))
                    damage = GetHitDamage() + (poison->GetEffect(EFFECT_0)->GetAmount() * poison->GetStackAmount());

            if (damage != 0)
                SetHitDamage(damage);
        }

        void Register()
        {
            OnEffectHitTarget += SpellEffectFn(spell_bot_scorching_breath_damage_SpellScript::CalculateDamage, EFFECT_0, SPELL_EFFECT_SCHOOL_DAMAGE);
        }
    };

    SpellScript* GetSpellScript() const
    {
        return new spell_bot_scorching_breath_damage_SpellScript();
    }
};

class go_halfus_whelp_cage : public GameObjectScript
{
    public:
        go_halfus_whelp_cage() : GameObjectScript("go_halfus_whelp_cage") { }

        bool OnGossipHello(Player* player, GameObject* go)
        {
            std::list<Creature*> creatures;
            GetCreatureListWithEntryInGrid(creatures, go, NPC_ORPHANED_EMERALD_WHELP, 1000.0f);

            if (creatures.empty())
               return false;
            
            for (std::list<Creature*>::iterator iter = creatures.begin(); iter != creatures.end(); ++iter)
                (*iter)->AI()->AttackStart(player);

            go->UseDoorOrButton();
            player->PlayerTalkClass->ClearMenus();

            return true;
        }
};

void AddSC_boss_halfus_wyrmbreaker()
{
    new at_bot_intro_1();
    new boss_halfus_wyrmbreaker();
    new npc_proto_behemoth();
    new npc_halfus_dragon();
    new npc_orphaned_whelp();
    new spell_bot_chain();
    new spell_proto_fireball();
    new spell_proto_fireball_barrage();
    new spell_halfus_stone_touch();
    new spell_bot_scorching_breath_damage();
    new go_halfus_whelp_cage();
}