import kotlinx.coroutines.experimental.delay
import kotlinx.coroutines.experimental.launch
import java.util.concurrent.TimeUnit

fun main(args: Array<String>) {
    val action = Action()
    val state = State()


	/*val process = ProcessBuilder("ls", "-la").start()
	process.inputStream.reader(Charsets.UTF_8).use {
		println(it.readText())
	}
	process.waitFor(10, TimeUnit.SECONDS)

	exitProcess(0)*/

    if (args.size != 3) { Print.printUsage() }

    Env.init(args)
    if (Env.nbClient > 0)
        while (Env.level < 7) state.walk(action)
    Env.client.close()

    // action.see().forEach { println(it) }

    /*  action.take(FOOD.value)

      action.inventory().forEach { key, value -> println("$key -> $value") }

      action.put(FOOD.value)

      action.inventory().forEach { key, value -> println("$key -> $value") }

      action.kick()*/

}
