-- Mounted Ironforge Mountaineer
SET @ENTRY := 12996;
SET @SOURCETYPE := 0;

DELETE FROM `smart_scripts` WHERE `entryorguid`=@ENTRY AND `source_type`=@SOURCETYPE;
UPDATE creature_template SET AIName="SmartAI" WHERE entry=@ENTRY LIMIT 1;
INSERT INTO `smart_scripts` (`entryorguid`,`source_type`,`id`,`link`,`event_type`,`event_phase_mask`,`event_chance`,`event_flags`,`event_param1`,`event_param2`,`event_param3`,`event_param4`,`action_type`,`action_param1`,`action_param2`,`action_param3`,`action_param4`,`action_param5`,`action_param6`,`target_type`,`target_param1`,`target_param2`,`target_param3`,`target_x`,`target_y`,`target_z`,`target_o`,`comment`) VALUES 
(@ENTRY,@SOURCETYPE,0,1,1,0,100,0,0,0,0,0,21,0,0,0,0,0,0,1,0,0,0,0.0,0.0,0.0,0.0,"Mounted Ironforge Mountaineer - Out Of Combat - Allow Combat Movement"),
(@ENTRY,@SOURCETYPE,1,0,61,0,100,0,0,0,0,0,20,0,0,0,0,0,0,1,0,0,0,0.0,0.0,0.0,0.0,"Mounted Ironforge Mountaineer - Out Of Combat - Start Auto Attack"),
(@ENTRY,@SOURCETYPE,2,3,4,0,100,0,0,0,0,0,11,6660,0,0,0,0,0,2,0,0,0,0.0,0.0,0.0,0.0,"Mounted Ironforge Mountaineer - On Aggro - Cast Shoot"),
(@ENTRY,@SOURCETYPE,3,0,61,0,100,0,0,0,0,0,23,1,0,0,0,0,0,1,0,0,0,0.0,0.0,0.0,0.0,"Mounted Ironforge Mountaineer - On Aggro - Increment Phase"),
(@ENTRY,@SOURCETYPE,4,5,9,0,100,0,5,30,2300,3900,11,6660,0,0,0,0,0,2,0,0,0,0.0,0.0,0.0,0.0,"Mounted Ironforge Mountaineer - At 5 - 30 Range - Cast Shoot"),
(@ENTRY,@SOURCETYPE,5,0,61,0,100,0,0,0,0,0,40,2,0,0,0,0,0,1,0,0,0,0.0,0.0,0.0,0.0,"Mounted Ironforge Mountaineer - At 5 - 30 Range - Display ranged weapon"),
(@ENTRY,@SOURCETYPE,6,7,9,0,100,0,25,80,0,0,21,1,0,0,0,0,0,1,0,0,0,0.0,0.0,0.0,0.0,"Mounted Ironforge Mountaineer - At 25 - 80 Range - Allow Combat Movement"),
(@ENTRY,@SOURCETYPE,7,0,61,0,100,0,0,0,0,0,20,1,0,0,0,0,0,1,0,0,0,0.0,0.0,0.0,0.0,"Mounted Ironforge Mountaineer - At 25 - 80 Range - Start Auto Attack"),
(@ENTRY,@SOURCETYPE,8,0,9,0,100,0,0,5,0,0,21,1,0,0,0,0,0,1,0,0,0,0.0,0.0,0.0,0.0,"Mounted Ironforge Mountaineer - At 0 - 5 Range - Allow Combat Movement"),
(@ENTRY,@SOURCETYPE,9,10,9,0,100,0,0,0,0,0,40,1,0,0,0,0,0,1,0,0,0,0.0,0.0,0.0,0.0,"Mounted Ironforge Mountaineer - At 0 - 5 Range - Display melee weapon"),
(@ENTRY,@SOURCETYPE,10,0,61,0,100,0,0,0,0,0,20,1,0,0,0,0,0,1,0,0,0,0.0,0.0,0.0,0.0,"Mounted Ironforge Mountaineer - At 0 - 5 Range - Start Auto Attack"),
(@ENTRY,@SOURCETYPE,11,12,9,0,100,0,5,15,0,0,21,0,0,0,0,0,0,1,0,0,0,0.0,0.0,0.0,0.0,"Mounted Ironforge Mountaineer - At 5 - 15 Range - Allow Combat Movement"),
(@ENTRY,@SOURCETYPE,12,0,61,0,100,0,0,0,0,0,20,0,0,0,0,0,0,1,0,0,0,0.0,0.0,0.0,0.0,"Mounted Ironforge Mountaineer - At 5 - 15 Range - Start Auto Attack"),
(@ENTRY,@SOURCETYPE,13,0,2,0,100,1,0,15,0,0,23,1,0,0,0,0,0,1,0,0,0,0.0,0.0,0.0,0.0,"Mounted Ironforge Mountaineer - At 15% HP - Increment Phase"),
(@ENTRY,@SOURCETYPE,14,15,2,0,100,0,0,15,0,0,21,1,0,0,0,0,0,1,0,0,0,0.0,0.0,0.0,0.0,"Mounted Ironforge Mountaineer - At 15% HP - Allow Combat Movement"),
(@ENTRY,@SOURCETYPE,15,0,61,0,100,0,0,0,0,0,25,0,0,0,0,0,0,0,0,0,0,0.0,0.0,0.0,0.0,"Mounted Ironforge Mountaineer - At 15% HP - Flee For Assist"),
(@ENTRY,@SOURCETYPE,16,0,7,0,100,0,0,0,0,0,40,1,0,0,0,0,0,1,0,0,0,0.0,0.0,0.0,0.0,"Mounted Ironforge Mountaineer - On Evade - Display melee weapon");