# RPG

Unreal(4.27)引擎开发的RPGDemo项目源代码


技能系统：继承AbilityBase的子类可重写三个函数，AbilityStart，AbilityTick，AbilityEnd，效果实现重写接口AbilityDamageInterface。
	技能与玩家交互用AbilityComponent组件，详情查看源码。  
技能组件实现：Source/RPG/AbilitySystem/Component/AbilityComponent.h  
技能父类：Source/RPG/AbilitySystem/Actor/AbilityBase.h  


任务系统：继承MasterQuest在蓝图中编辑任务，其中StartList中要标明开始任务序号。
	任务与玩家交互用QuestComponent组件，详情查看源码。  
任务对象实现：Source/RPG/QuestSystem/Actor/MasterQuest.h  
任务组件实现：Source/RPG/QuestSystem/Component/QuestComponent.h  


对话系统：继承FDialogueConten创建DataTable来编辑对话，在NPC蓝图中引用DataTable，其中DataTable内衔接任务接取。  
存档系统：在有遍历场景中继承SaveLoadInterface接口的Actor保存和载入属性。  
存档管理器：Source/RPG/GameSystem/LoadManager.h  
存档类：Source/RPG/LoadSystem/SaveData/RPGSaveGame.h  

对象池：源代码有封装好的Actor对象池和Widget对象池，在使用时，运用宏CREATE_ACTOR/WIDGET_POOL传入C++或者蓝图类来添加到对象池。  
Actor对象池的实现源代码：Source/RPG/GameSystem/ActorPool.h  
Widget对象池的实现源代码：Source/RPG/GameSystem/WidgetPool.h  

AI行为树节点：源码里有几个比较常用的巡逻，随机移动，攻击的TaskNode或者Server。  

content文件夹：链接：链接：https://pan.baidu.com/s/1YM_rf1EzsAF6PgDUkmjWFw?pwd=9au4
