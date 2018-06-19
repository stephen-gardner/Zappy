import Print.printError
import Print.printMessage
import java.io.DataInputStream
import java.io.DataOutputStream
import java.io.IOException
import kotlin.system.exitProcess

const val MESSAGE_ELEVATION_START = "elevation in progress"
const val MESSAGE_ELEVATION_FINISH = "current level : "
const val BROADCAST_CALLING = "calling-"
const val BROADCAST_COMING = "coming"
const val MESSAGE_BROADCAST = "message "
const val MESSAGE_DEATH = "death"

object Message {

	fun getMessage() : String {
		val msg = StringBuffer("")

		try {
			val input = DataInputStream(Env.client.getInputStream()!!)
			while (true)
			{
				val c = input.read().toChar()
				if (c == '\n') break
				msg.append(c)
			}

		} catch (e: IOException) {
			printError("${e.message}")
		} catch (e: OutOfMemoryError) {
			printError("${e.message}")
		}

		var res = msg.toString()

		if (res.startsWith(MESSAGE_BROADCAST)) {
			res = res.substring(MESSAGE_BROADCAST.length, res.length)
			res.split(",").map { it.trim() }.also {
				if (it.size != 2) printError(MESSAGE_BROADCAST)

				if (it[0].startsWith(BROADCAST_CALLING)) {
					println(BROADCAST_CALLING)
					val origin = try {
						it[0].substring(BROADCAST_CALLING.length, it[0].length).toInt()
					} catch (e: NumberFormatException) {
						printError(BROADCAST_CALLING); -1
					}

					val message = try {
						it[1].toInt()
					} catch (e: NumberFormatException) {
						printError(MESSAGE_BROADCAST); 0
					}

					if (Env.level == message) Env.broadcastCalling = origin

					println("$BROADCAST_CALLING ${Env.broadcastCalling}")
				}
				if (it[0].startsWith(BROADCAST_COMING)) {
					val message = try {
						it[1].toInt()
					} catch (e: NumberFormatException) {
						printError(MESSAGE_BROADCAST); 0
					}

					if (Env.level == message) Env.broadcastComing++
				}
			}
			return getMessage()
		}

		if (res == MESSAGE_DEATH) {
			printMessage(MESSAGE_DEATH)
			exitProcess(0)
		}
		return res
	}

	fun sendMessage(msg: String) = try {
		msg.forEach { DataOutputStream(Env.client.getOutputStream()!!).writeByte(it.toInt()) }
	} catch (e: IOException) {
		printError("${e.message}")
	}
}