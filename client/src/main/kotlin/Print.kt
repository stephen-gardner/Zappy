import kotlin.system.exitProcess

object Print {
    fun printUsage() {
        println("Usage: ./client \n\t-n <team> \n\t-p <port> \n\t[-h <hostname>]")
        exitProcess(-1)
    }

    fun printError(msg: String) {
        println("ERROR : $msg")
        exitProcess(-1)
    }

    fun printMessage(msg: String) = println(msg)

}