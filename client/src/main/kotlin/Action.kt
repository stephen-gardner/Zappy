import CMD.ADVANCE
import CMD.BROADCAST
import CMD.CONNECT_NBR
import CMD.FORK
import CMD.INCANTATION
import CMD.INVENTORY
import CMD.KICK
import CMD.LEFT
import CMD.PUT
import CMD.RIGHT
import CMD.SEE
import CMD.TAKE
import CODE.KO
import CODE.OK
import Print.printError
import Resource.RESOURCE
import Resource.getStonesMap
import kotlinx.coroutines.experimental.launch
import java.lang.Thread.sleep
import java.util.*

class Action {

	fun advance() {
        Message.sendMessage(ADVANCE).also { print(ADVANCE.toUpperCase()) }
        if (Message.getMessage() != OK.value) printError(ADVANCE)
    }

    fun turnRight() {
        Message.sendMessage(RIGHT).also { print(RIGHT.toUpperCase()) }
        if (Message.getMessage() != OK.value) printError(RIGHT)
    }

    fun turnLeft() {
        Message.sendMessage(LEFT).also { print(LEFT.toUpperCase()) }
        if (Message.getMessage() != OK.value) printError(LEFT)
    }

    fun see(): List<List<String>> {
        Message.sendMessage(SEE)

        val msg = Message.getMessage()
        val list: MutableList<List<String>> = mutableListOf()
        val resources = RESOURCE.values().map { it.value }

        if (msg.length < 2 || msg.first() != '{' || msg.last() != '}') printError(INCANTATION)

        msg.substring(1, msg.length - 1)
                .split(",")
                .map { it.trim() }
                .forEach {
                    list.add((it.split(" ").filter { resources.contains(it.trim()) })) }
        return list.toMutableList()
    }

    fun inventory(): MutableMap<String, Int> {
        Message.sendMessage(INVENTORY)

        val msg = Message.getMessage()
        val inventory = getStonesMap()

        if (msg.length < 2 || msg.first() != '{' || msg.last() != '}') printError(INVENTORY)

        msg
                .substring(1, msg.length - 1)
                .split(",")
                .map { it.trim() }
                .forEach {
                    it.split(" ").map { it.trim() }.also {
                        if (it.size == 2 && inventory.containsKey(it[0]))
                            inventory[it[0]] = try {
                                it[1].toInt()
                            } catch (e: NumberFormatException) {
                                printError(INVENTORY); 0
                            } 
                    }
                }

        return inventory
    }

    fun take(resource: String) : CODE {
        Message.sendMessage("$TAKE $resource\n")
        return if (Message.getMessage().also { if (it != OK.value && it != KO.value) printError(TAKE) } == OK.value) CODE.OK else CODE.KO
    }

    fun put(resource: String) : CODE {
        Message.sendMessage("$PUT $resource\n")
        return if (Message.getMessage().also { if (it != OK.value && it != KO.value) printError(PUT) } == OK.value) CODE.OK else CODE.KO
    }

    fun kick() : CODE {
        Message.sendMessage(KICK)
        return if (Message.getMessage().also { if (it != OK.value && it != KO.value) printError(PUT) } == OK.value) CODE.OK else CODE.KO
    }

	fun broadcastCalling() {
		println("broadcastCalling")
		broadcast("$BROADCAST_CALLING${Env.level}")
	}

	fun broadcastComing() {
		val currentLevel = Env.level
		println("broadcastComing")
		broadcast(BROADCAST_COMING)
		while (Env.level != currentLevel)
			incantation()
	}

    private fun broadcast(msg: String) {
        Message.sendMessage(BROADCAST + msg + "\n")
		val message = Message.getMessage()
		if (message != OK.value) printError(BROADCAST)
    }

	fun incantation() {
		println(INCANTATION)
        Message.sendMessage(INCANTATION)

		var res = Message.getMessage()

		if (res == MESSAGE_ELEVATION_START)
			res = Message.getMessage()

		if (res.startsWith(MESSAGE_ELEVATION_FINISH)) res = res.substring(MESSAGE_ELEVATION_FINISH.length, res.length)

		try {
			Env.level = res.toInt()
			Env.broadcastComing = 1
			println("END ${Env.level}")
        } catch (e: NumberFormatException) { printError(INCANTATION) }
    }

    fun fork() {
        Message.sendMessage(FORK)
        if (Message.getMessage() != OK.value) printError(FORK)
		//connectPlayer()
    }

	private fun connectPlayer() {
		if (connectNbr() > 0) {
			launch {
				sleep(5000)
				val code = Random().nextInt()
				try {
					val proc = ProcessBuilder("java", "-jar", "../../../build/libs/client-1.0.jar", "red", "4242", "localhost").start()
					Scanner(proc.inputStream).use {
						while (it.hasNextLine()) println("$code: " + it.nextLine())
					}
				}catch (e: Exception) {
					println("ERR ${e.message}")}
			}
		//	println("NEW PLAYER")
		}
	}

	fun connectNbr() : Int {
        Message.sendMessage(CONNECT_NBR)
        return try {
            Message.getMessage().toInt()
        } catch (e: NumberFormatException) {
            printError(INVENTORY); 0
        }
    }

}