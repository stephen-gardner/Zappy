
import java.util.*

var num = 1000


fun main(args: Array<String>) {

	val runnable = Runnable {
		Thread.sleep(2000)
		println("Hello ${Thread.currentThread().name}")
	}

	runnable.run()

	val thread = Thread(runnable)
	thread.start()
	/*println("##")
	for (i in 1..20) {
		Thread(
			{
				val code = num++
				try {
					val proc = ProcessBuilder("java", "-jar", "../../../build/libs/client-1.0.jar", "red", "4242", "localhost").start()
					Scanner(proc.inputStream).use {
						while (it.hasNextLine()) println("$code: " + it.nextLine())
					}
				}catch (e: Exception) {
					println("ERR ${e.message}")}
			}
		).start()
	}
	println("##")*/
}