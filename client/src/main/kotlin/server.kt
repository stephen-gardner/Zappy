import java.net.*
import java.io.*

fun main(args: Array<String>) {
    val serverSocket: ServerSocket

    serverSocket = ServerSocket(args[0].toInt())

    while (true) {
        try {
         //   println("Waiting for client on port " + serverSocket.localPort + "...")
            val server = serverSocket.accept()

          //  println("Just connected to " + server.remoteSocketAddress)
            val im = DataInputStream(server.getInputStream())
            val out = DataOutputStream(server.getOutputStream())


            out.writeUTF("Welcome")
            println(im.readUTF())
            println(im.readUTF())
            println(im.readUTF())
            println(im.readUTF())


            server.close()

        } catch (s: SocketTimeoutException) {
            println("Socket timed out!")
            break
        } catch (e: IOException) {
            //println("Ex " + e.message)
            break
        }

    }
}