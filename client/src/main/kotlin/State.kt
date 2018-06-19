import CODE.OK
import Resource.RESOURCE

class State {

	private val levels: Array<Map<String, Int>> = arrayOf(
		mapOf("linemate" to 1, "deraumere" to 0, "sibur" to 0, "mendiane" to 0, "phiras" to 0, "thystame" to 0),
		mapOf("linemate" to 1, "deraumere" to 1, "sibur" to 1, "mendiane" to 0, "phiras" to 0, "thystame" to 0),
		mapOf("linemate" to 2, "deraumere" to 0, "sibur" to 1, "mendiane" to 0, "phiras" to 2, "thystame" to 0),
		mapOf("linemate" to 1, "deraumere" to 1, "sibur" to 2, "mendiane" to 0, "phiras" to 1, "thystame" to 0),
		mapOf("linemate" to 1, "deraumere" to 2, "sibur" to 1, "mendiane" to 3, "phiras" to 0, "thystame" to 0),
		mapOf("linemate" to 1, "deraumere" to 2, "sibur" to 3, "mendiane" to 0, "phiras" to 1, "thystame" to 0),
		mapOf("linemate" to 2, "deraumere" to 2, "sibur" to 2, "mendiane" to 2, "phiras" to 2, "thystame" to 1)
	)

	private val players = arrayOf(1, 2, 2, 4, 4, 6, 6)

	fun walk(action: Action) {
		var floor = 0
		var i = 0
		val inventory = action.inventory()
		val see = action.see().also { println("walk : $it") }
		if (action.connectNbr() > 0) action.fork()

		while (i < see.size) {
			println("I $i - Floor $floor")

			println("CENTER ${action.see()}")
			doCollecte(see[i], inventory, action)
			println("CENTER ${action.see()}")
			if (canIncantate(action, inventory)) break

			println("RIGHT ${action.see()}")
			collectRight(i, floor, see, inventory, action)
			println("RIGHT ${action.see()}")
			if (canIncantate(action, inventory)) break

			println("LEFT ${action.see()}")
			collectLeft(i, floor, see, inventory, action)
			println("LEFT ${action.see()}")
			if (canIncantate(action, inventory)) break

			i = ++floor * (floor + 1)
			println("A ${action.see()}")
			action.advance()
			println("B ${action.see()}")
		}
	}

	private fun canIncantate(action: Action, inventory: MutableMap<String, Int>): Boolean {
		println("canIncantate ${action.inventory()} - Level ${Env.level}")
		return when {
			checkBroadcast(action) -> return goBroadcast(action)
			checkInventory(action, inventory) -> return doBroadcast(action)
			else -> false
		}
	}

	private fun checkInventory(action: Action, inventory: MutableMap<String, Int>): Boolean {
		for ((res, value) in levels[Env.level - 1])
			if (inventory.containsKey(res) && inventory[res]!! < value) return false
		return true
	}

	private fun checkBroadcast(action: Action): Boolean {
		return Env.broadcastCalling != -1
	}


	private fun goBroadcast(action: Action): Boolean {
	//	println("goBroadcast")

		last@ while (Env.broadcastCalling != -1) {
			when (Env.broadcastCalling) {
				0 -> {
					action.broadcastComing()
					break@last
				}
				1 -> {
					action.advance()
					action.take(Resource.RESOURCE.FOOD.value)
				}
				2 -> {
					action.advance()
					action.take(Resource.RESOURCE.FOOD.value)
					action.turnLeft()
					action.advance()
					action.take(Resource.RESOURCE.FOOD.value)
				}
				3 -> {
					action.turnLeft()
					action.advance()
					action.take(Resource.RESOURCE.FOOD.value)
				}
				4 -> {
					action.turnLeft()
					action.advance()
					action.take(Resource.RESOURCE.FOOD.value)
					action.turnLeft()
					action.advance()
					action.take(Resource.RESOURCE.FOOD.value)
				}
				5 -> {
					action.turnLeft()
					action.turnLeft()
					action.advance()
					action.take(Resource.RESOURCE.FOOD.value)
				}
				6 -> {
					action.turnRight()
					action.advance()
					action.take(Resource.RESOURCE.FOOD.value)
					action.turnRight()
					action.advance()
					action.take(Resource.RESOURCE.FOOD.value)
				}
				7 -> {
					action.turnRight()
					action.advance()
					action.take(Resource.RESOURCE.FOOD.value)
				}
				8 -> {
					action.advance()
					action.take(Resource.RESOURCE.FOOD.value)
					action.turnRight()
					action.advance()
					action.take(Resource.RESOURCE.FOOD.value)
				}
			}
		}
		Env.broadcastCalling = -1
		return false
	}

	private fun doBroadcast(action: Action): Boolean {
		levels[Env.level - 1].forEach { res, nbr ->
			var n = nbr
			while (n-- > 0) {
				//println("PUT $res")
				action.put(res)
			}
		}
	//	println("doBroadcast ${action.see()[0]}")
		while (Env.broadcastComing < players[Env.level - 1])
			action.broadcastCalling()
		action.incantation()
		return true
	}

	private fun doCollecte(resources: List<String>, inventory: MutableMap<String, Int>, action: Action) {
		println("doCollecte $resources - SEE ${action.see()[0]}")
		resources.forEach {
			if (Resource.getMaxStones(RESOURCE.valueOf(it.toUpperCase())) > inventory[it]!!)
			{
		//		println("TAKE $it")
				if (action.take(it) == OK) inventory[it] = inventory[it]!! + 1
			}
		}
	}

	private fun collectRight(_indice: Int, _floor: Int, see: List<List<String>>, inventory: MutableMap<String, Int>, action: Action) {
		var indice = _indice
		var floor = _floor

		action.turnRight()
		while (floor-- > 0) {
			action.advance()
			doCollecte(see[++indice], inventory, action)
		}
		action.turnLeft()
		action.turnLeft()
		floor = _floor
		while (floor-- > 0) action.advance()
		action.turnRight()
	}

	private fun collectLeft(_indice: Int, _floor: Int, see: List<List<String>>, inventory: MutableMap<String, Int>, action: Action) {
		var indice = _indice
		var floor = _floor

		action.turnLeft()
		while (floor-- > 0) {
			action.advance()
			doCollecte(see[--indice], inventory, action)
		}
		action.turnRight()
		action.turnRight()
		floor = _floor
		while (floor-- > 0) action.advance()
		action.turnLeft()
	}

}