/*////////////////////////////////////////////////////////////////////////////

This file is part of DynItemInst.

Copyright � 2015 David Goeth

All Rights reserved.

THE WORK (AS DEFINED BELOW) IS PROVIDED
UNDER THE TERMS OF THIS CREATIVE COMMONS
PUBLIC LICENSE ("CCPL" OR "LICENSE").
THE WORK IS PROTECTED BY COPYRIGHT AND/OR
OTHER APPLICABLE LAW. ANY USE OF THE WORK
OTHER THAN AS AUTHORIZED UNDER THIS LICENSE
OR COPYRIGHT LAW IS PROHIBITED.

BY EXERCISING ANY RIGHTS TO THE WORK PROVIDED
HERE, YOU ACCEPT AND AGREE TO BE BOUND BY THE
TERMS OF THIS LICENSE. TO THE EXTENT THIS
LICENSE MAY BE CONSIDERED TO BE A CONTRACT,
THE LICENSOR GRANTS YOU THE RIGHTS CONTAINED
HERE IN CONSIDERATION OF YOUR ACCEPTANCE OF
SUCH TERMS AND CONDITIONS.

Full license at http://creativecommons.org/licenses/by-nc/3.0/legalcode

/////////////////////////////////////////////////////////////////////////////**/

#ifndef __ObjectManager_H__
#define __ObjectManager_H__

#include "DynInstance.h"
#include <HookManager.h>
#include <map>
#include <queue>
#include "oCMobContainer.h"
#include "AdditMemory.h"
#include "zCPar_SymbolTable.h"
#include <functional>
#include "api/g2/ocnpcinventory.h"

/**
 * This class is responsible for managing DynInstance and AdditMemory objects.
 */

typedef DWORD (__thiscall* ZCPAR_SYMBOL_CONSTRUCTOR)(void* thizz);
static ZCPAR_SYMBOL_CONSTRUCTOR zCPar_SymbolConstructor = (ZCPAR_SYMBOL_CONSTRUCTOR)0x007A1690;

class ObjectManager {
public:
	/**
	 * New instance ids will have numbers which will start at this number in ascending order.
	 */
	//static const int INSTANCE_BEGIN = 5000000;

	static const int SPECIAL_ADDIT_BEGIN = 1500000000;

	/**
	 * This number will be used to mark zCPar_Symbols of new instances.
	 */
	static const int ZCPAR_SYMBOL_MARK_ID = -6666666;

	/**
	 * A structure for storing relevant zCPar_Symbol information. Additionally it  provides 
	 * with the container field a field for associating a new instance.
	 */
	struct ParserInfo
	{
		int newInstanceId;
		int oldInstanceId;
		std::string name;
		int bitfield;
		DynInstance* container;
	};
public:

	/**
	 * \return the current instance of this class.
	 */
	static ObjectManager* getObjectManager();

	/**
	 * Deletes the current object manager (if one exists) and removes all dynamic instances.
	 */
	static void release();

	/**
	 * Calls oCItem::InitByScript(int, int), which initializes the given oCItem by its instance id.
	 * \param item The item to be initialized
	 * \param inst The instance id the item should be initialized with
	 * \param amount The amount of instances the item should have.
	 */
	static void __thiscall oCItemInitByScript (oCItem* item, int inst, int amount);

	/**
	 * \return The list of all oCMobContainers the current oCGame contains.
	 */
	static std::list<oCMobContainer*>* getMobContainers();

	void createNewInstanceWithoutExistingId(oCItem* item, int key);

	//void createNewInstanceForExistingId(oCItem* item, int instanceId);
	/**
			 * Creates a new oCItem instance which will be initialized with the members of the provided oCItem.
			 * \param item The item to use for instance creation.
			 * \return The instance id respectively the index of the new created zCPar_Symbol.
			 */
	int createNewInstanceId(oCItem* item);

	/**
	 * Assigns a given Item to a given instance id but only if the Item wasn't assigned to one id already.
	 */
	void createInstanceById(int id, DynInstance* item);

	/**
	 * Assigns an oCItem to an specified instance id. True will be returned if the assignment
	 * was successful, i.e. the instance id was found in the map of all registered instance ids.
	 * After assignment the item will be initialized with its new instance id.
	 * \param item The item which should be assigned
	 * \param id The instance id the item should be assigned to.
	 * \return Was the assignment successful?
	 */
	bool assignInstanceId(oCItem* item, int id);
	
	
	static void oCItemOperatorDelete(oCItem* item);

	static bool assignInstanceId2(oCItem* item, int id);

	/**
	 * Provides the instance id of a given oCItem.
	 * \param item The item to get the dynamic instance from.
	 * \return The dynamic instance id of the given item. If the oCItem has no dynamic instance, 
	 * NULL will be returned.
	 */
	int getDynInstanceId(oCItem* item);

	/**
	 * Sets the instance id of a given oCItem. If the oCItem has no dynamic instance, 
	 * no changes will be performed.
	 * \param item The item, the instance id of which should be set.
	 * \param The dynamic instance id the item should be assigned to.
	 */
	void setDynInstanceId(oCItem* item, int id);

	/**
	 * \param instanceId The instance id to get the dynamic instance from.
	 * \return the Item which is associated with the given instance id. NULL will be returned
	 * if no connection could be found.
	 */
	DynInstance* getInstanceItem(int instanceId);

	/**
	 * Stores all new created instances at a directory described by 'directoryPath'. If the provided 
	 * directory path doesn't exists, the directory structure will be created.
	 * The Savegame will have the name defined by 'filename'.
	 */
	void saveNewInstances(char* directoryPath, char* filename);

	/**
	 * Loads new instances from a file defined by filePath.
	 * \param filePath The path to the file to be loaded.
	 */
	void loadNewInstances(char* filePath);

	/**
	 * Removes all new created instances and resets internal state.
	 */
	void releaseInstances();

	/**
	 * Sets the parent instance id for a given instance id.
	 * For definition of parent instance id see: \ref symIndInst
	 * \param instanceId The instance id of which the parent instance id should be set
	 * \param value The the parent instance id to be set
	 */
	void setParentInstanceId(int instanceId, int value);

	/**
	 * Provides the parent instance id for a given instance id.
	 * For definition of parent instance id see: \ref symIndInst
	 * \param instanceId The instance id of which the parent instance id should be returned.
	 * \return the parent instance id of the given instance id.
	 */
	int getParentInstanceId(int instanceId);

	/**
	 * Sets the instance id for a given oCItem.
	 * For definition of an instance id see: \ref symIndInst
	 * \param item The item of which the instance id should be set.
	 * \param instanceId The new instance id
	 */
	static void setInstanceId(oCItem* item, int instanceId);

	/**
	 * Provides the instance id for a given oCItem.
	 * For definition of an instance id see: \ref symIndInst
	 * \param item The item to get the instane id from.
	 * \return The instance id of the given item
	 */
	static int getInstanceId(oCItem& item);

	/**
	 * Provides the directory path of a savegame folder with a given slot number.
	 * \param saveGameSlotNumber The slot number to get the directory path from.
	 * \return The directory path from the savegame directory having the provided slot number.
	 */
	static std::string getSaveGameDirectoryPath(int saveGameSlotNumber);

	/**
	 * \return The directory path of the savegame folder named 'current'. 
	 */
	static std::string getCurrentDirectoryPath();

	/**
	 * Inits a given oCItem with a new instance id. The return value indicates if initialization
	 * was successful.
	 * \param item The item to initialized with a dynamic instance.
	 * \param index The zCPar_Symbol index of the dynamic instance. For more information about
	 * the connection between symbol indices and instance ids see: \ref symIndInst
	 * \return Was the initialization successful?
	 */
	bool InitItemWithDynInstance(oCItem* item, int index);

	/**
	 * Checks whether the given oCItem has a dynamic instance.
	 * \param item The item to check
	 * \return Has the given item a dynamic instance?
	 */
	bool IsModified(oCItem* item);

	/**
	 * Checks whether the given instance id is a dynamic one.
	 * \param instanceId The instance id to check
	 * \return Has the given item a dynamic instance?
	 */
	bool IsModified(int instanceId);

	/**
	 * Provides the zCPar_Symbol with index 'index' if the provided index refers to a dynamic instance.
	 * Otherwise NULL will be returned.
	 * For more information between the connection of parser symbols and dynamic instances see: \ref symIndInst
	 * \param index The index of the parser symbol.
	 * \return The parser symbol of the given index.
	 */
	zCPar_Symbol* getSymbolByIndex(int index);

	/**
	 * Provides the zCPar_Symbol with name  'symbolName'. Symbols stored by this object manager 
	 * represent dynamic instances. If no associated parser symbol could be found, 
	 * NULL will be returned.
	 * For more information on the connection between parser symbols and dynamic instances see: \ref symIndInst
	 * \param name The name of the parser symbol.
	 * \return The parser symbol of the given index.
	 */
	zCPar_Symbol* getSymbolByName(zSTRING symbolName);

	/**
	 * Provides the index of an given symbol name if an associated zCPar_Symbol is found.
	 * Otherwise NULL will be returned.
	 * For more information on the connection between parser symbols and dynamic instances see: \ref symIndInst
	 * \param symbolName The name of the parser symbol.
	 * \return The index of the symbol with the provided symbol name.
	 */
	int getIndexByName(zSTRING symbolName);

	void updateIkarusSymbols();

	void callForAllItems(void(*func)(void* obj, void* param, oCItem*), void* obj, void* param, oCItem** stopIfNotNullItem = NULL);
	static void callForInventoryItems(void(*func)(void* obj, void* param, oCItem*), void* obj, void* param, oCNpc * npc);
	static void callForAllContainerItems(void(*func)(void* obj, void* param, oCItem*), void* obj, void* param, oCMobContainer* container);
	static void callForAllWorldItems(void(*func)(void* obj, void* param, oCItem*), void* obj, void* param);

	int getInstanceBegin() const;

	static int * getParserInstanceCount();

	static int getIdForSpecialPurposes();

	/**
	* Checks whether the specified oCItem is in the game world's list registered
	* \param item The oCItem to check
	* \return true if the item is added into the world
	*/
	static bool isItemInWorld(oCItem* item);

	/**
	 * Searches an oCItem by its instance id in the world, in all containers and all inventories.
	 * \param instanceId the instance id for searching the oCItem
	 * \return The first oCItem found
	 */
	oCItem* getItemByInstanceId(int instanceId);

	static void oCItemSaveInsertEffect(oCItem* item);
	static void oCItemSaveRemoveEffect(oCItem* item);
	bool isDynamicInstance(int instanceId);
	static int* getRefCounter(oCItem* item);


	static zCListSort<oCItem>* getInvItemByInstanceId(oCNpcInventory * inventory, int instanceId);
	
	int getSlotNumber(oCNpcInventory* inventory, oCItem* item);
	
	oCItem* searchItemInInvbyInstanzValue(oCNpcInventory* inventory, int searchValue);

	static g2ext_extended::zCPar_SymbolTable* zCParserGetSymbolTable(void* parser);

	/**
	* Inits an oCItem by its (previously) assigned (new) instance id. The return value indicates
	* if the initialization was successful. E.g. if no registered instance id was found, this method
	* will return false.
	* \param item The item to initialize
	* \return Was the initialization successful?
	*/
	bool initByNewInstanceId(oCItem* item);

private:

	/**
	 * Stores relevant information for signing instances.
	 */
	struct InstanceInfo {
		int id;
		int parentId;
	};

	/**
	 * singleton of this class
	 */
	static ObjectManager* instanz;

	// <int instanceId, Item* item>
	std::map<int, DynInstance*> instanceMap;

	std::vector<ParserInfo> indexZCParSymbolNameMap;
	std::map<int, zCPar_Symbol*> newInstanceToSymbolMap;
	std::map<std::string, zCPar_Symbol*> nameToSymbolMap;
	std::map<std::string, int> nameToIndexMap;

	std::stringstream logStream;
	int ObjectManager::instanceBegin;

private:

	/**
	 * Creates a new object manager.
	 */
	ObjectManager();

	static zCPar_Symbol* createNewSymbol(ParserInfo* old);
	bool addSymbolToSymbolTable(zCPar_Symbol* symbol);
	DynInstance * createNewInstanceItem(int instanceId);
	void updateContainerItem(ObjectManager::ParserInfo* info);
	/**
	 * Logs the current status of the provided zCPar_Symbol pointer.
	 * \param sym The symbol to log.
	 */
	void logSymbolData(zCPar_Symbol* sym);

	/**
	 * Creates for all registered instances zCPar_Symbols and registers the latter.
	 */
	void createParserSymbols();

	zCPar_Symbol * createNewSymbol(int instanceId, zCPar_Symbol * old) const;

	/**
	 * Calculates a new key for an AdditMemory object. The argument isHeroItem specifies whether
	 * the key is used for an item of the inventory of the player's character.
	 * \param isHeroItem Should the key be used for an item in the player's inventory?
	 * \return The new created additional memory key. 
	 */
	int calcAdditKey(bool isHeroItem) const;

	static void* __cdecl gothic2OperatorNew(size_t size);
};

#endif __ObjectManager_H__