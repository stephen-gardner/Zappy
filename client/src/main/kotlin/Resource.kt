import java.lang.Integer.MAX_VALUE

object Resource {

    private val maxStones = mapOf(
            "linemate" to mapOf(
                    1 to 9,
                    2 to 8,
                    3 to 7,
                    4 to 5,
                    5 to 4,
                    6 to 3,
                    7 to 2),
            "deraumere" to mapOf(
                    1 to 8,
                    2 to 8,
                    3 to 7,
                    4 to 7,
                    5 to 6,
                    6 to 4,
                    7 to 2),
            "sibur" to mapOf(
                    1 to 10,
                    2 to 10,
                    3 to 9,
                    4 to 8,
                    5 to 6,
                    6 to 5,
                    7 to 2),
            "mendiane" to mapOf(
                    1 to 5,
                    2 to 5,
                    3 to 5,
                    4 to 5,
                    5 to 5,
                    6 to 2,
                    7 to 2),
            "phiras" to mapOf(
                    1 to 6,
                    2 to 6,
                    3 to 6,
                    4 to 4,
                    5 to 3,
                    6 to 3,
                    7 to 2),
            "thystame" to mapOf(
                    1 to 1,
                    2 to 1,
                    3 to 1,
                    4 to 1,
                    5 to 1,
                    6 to 1,
                    7 to 1)
    )

    enum class RESOURCE(val value: String) {
        LINEMATE("linemate"),
        DERAUMERE("deraumere"),
        SIBUR("sibur"),
        MENDIANE("mendiane"),
        PHIRAS("phiras"),
        THYSTAME("thystame"),
        FOOD("food"),
        PLAYER("player")
    }

    fun getStones() : List<String> {
        return listOf(RESOURCE.LINEMATE.value, RESOURCE.DERAUMERE.value, RESOURCE.SIBUR.value, RESOURCE.MENDIANE.value, RESOURCE.PHIRAS.value, RESOURCE.THYSTAME.value, RESOURCE.FOOD.value)
    }

    fun getStonesMap() : MutableMap<String, Int>{
        return mutableMapOf(RESOURCE.LINEMATE.value to 0, RESOURCE.DERAUMERE.value to 0, RESOURCE.SIBUR.value to 0, RESOURCE.MENDIANE.value to 0, RESOURCE.PHIRAS.value to 0, RESOURCE.THYSTAME.value to 0, RESOURCE.FOOD.value to 0, RESOURCE.PLAYER.value to 0)
    }

    fun isStone(stone : String) : Boolean = getStones().contains(stone)

    fun getMaxStones(resource: RESOURCE): Int {
        return when {
            maxStones.containsKey(resource.value) -> maxStones[resource.value]!![Env.level]!!
            resource== RESOURCE.FOOD -> MAX_VALUE
            resource == RESOURCE.PLAYER -> 0
            else -> {
                Print.printError("getMaxStones")
                0
            }
        }
    }

}



