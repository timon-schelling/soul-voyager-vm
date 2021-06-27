#!/usr/bin/env kscript

import java.io.*

enum class Instruction(val code: UByte) {
    HLT(0.toUByte()),

    MOV_REG_TO_REG(1.toUByte()),
    MOV_ADDRESS_TO_REG(2.toUByte()),
    MOV_REGADDRESS_TO_REG(3.toUByte()),
    MOV_REG_TO_ADDRESS(4.toUByte()),
    MOV_REG_TO_REGADDRESS(5.toUByte()),
    MOV_NUMBER_TO_REG(6.toUByte()),
    MOV_NUMBER_TO_ADDRESS(7.toUByte()),
    MOV_NUMBER_TO_REGADDRESS(8.toUByte()),

    ADD_REG_TO_REG(9.toUByte()),
    ADD_REGADDRESS_TO_REG(10.toUByte()),
    ADD_ADDRESS_TO_REG(11.toUByte()),
    ADD_NUMBER_TO_REG(12.toUByte()),

    SUB_REG_FROM_REG(13.toUByte()),
    SUB_REGADDRESS_FROM_REG(14.toUByte()),
    SUB_ADDRESS_FROM_REG(15.toUByte()),
    SUB_NUMBER_FROM_REG(16.toUByte()),

    INC_REG(17.toUByte()),

    DEC_REG(18.toUByte()),

    CMP_REG_WITH_REG(19.toUByte()),
    CMP_REGADDRESS_WITH_REG(20.toUByte()),
    CMP_ADDRESS_WITH_REG(21.toUByte()),
    CMP_NUMBER_WITH_REG(23.toUByte()),

    JMP_REGADDRESS(24.toUByte()),
    JMP_ADDRESS(25.toUByte()),

    JC_REGADDRESS(26.toUByte()),
    JC_ADDRESS(27.toUByte()),

    JNC_REGADDRESS(28.toUByte()),
    JNC_ADDRESS(29.toUByte()),

    JZ_REGADDRESS(30.toUByte()),
    JZ_ADDRESS(31.toUByte()),

    JNZ_REGADDRESS(32.toUByte()),
    JNZ_ADDRESS(33.toUByte()),

    JA_REGADDRESS(34.toUByte()),
    JA_ADDRESS(35.toUByte()),

    JNA_REGADDRESS(36.toUByte()),
    JNA_ADDRESS(37.toUByte()),

    PUSH_REG(38.toUByte()),
    PUSH_REGADDRESS(39.toUByte()),
    PUSH_ADDRESS(40.toUByte()),
    PUSH_NUMBER(41.toUByte()),

    POP_REG(42.toUByte()),
    POP_REGADDRESS(43.toUByte()),
    POP_ADDRESS(44.toUByte()),

    CALL_REGADDRESS(45.toUByte()),
    CALL_ADDRESS(46.toUByte()),

    RET(47.toUByte()),

    MUL_REG(48.toUByte()),
    MUL_REGADDRESS(49.toUByte()),
    MUL_ADDRESS(50.toUByte()),
    MUL_NUMBER(51.toUByte()),

    DIV_REG(52.toUByte()),
    DIV_REGADDRESS(53.toUByte()),
    DIV_ADDRESS(54.toUByte()),
    DIV_NUMBER(55.toUByte()),

    AND_REG_WITH_REG(56.toUByte()),
    AND_REGADDRESS_WITH_REG(57.toUByte()),
    AND_ADDRESS_WITH_REG(58.toUByte()),
    AND_NUMBER_WITH_REG(59.toUByte()),

    OR_REG_WITH_REG(60.toUByte()),
    OR_REGADDRESS_WITH_REG(61.toUByte()),
    OR_ADDRESS_WITH_REG(62.toUByte()),
    OR_NUMBER_WITH_REG(63.toUByte()),

    XOR_REG_WITH_REG(64.toUByte()),
    XOR_REGADDRESS_WITH_REG(65.toUByte()),
    XOR_ADDRESS_WITH_REG(66.toUByte()),
    XOR_NUMBER_WITH_REG(67.toUByte()),

    NOT_REG(68.toUByte()),

    SHL_REG_WITH_REG(69.toUByte()),
    SHL_REGADDRESS_WITH_REG(70.toUByte()),
    SHL_ADDRESS_WITH_REG(71.toUByte()),
    SHL_NUMBER_WITH_REG(72.toUByte()),

    SHR_REG_WITH_REG(73.toUByte()),
    SHR_REGADDRESS_WITH_REG(74.toUByte()),
    SHR_ADDRESS_WITH_REG(75.toUByte()),
    SHR_NUMBER_WITH_REG(76.toUByte()),
}

data class Label(val name: String, val location: Int)
data class LabelUse(val name: String, val location: Int)

fun UInt.toUBytes() = listOf((this shr 24).toUByte(), (this shr 16).toUByte(), (this shr 8).toUByte(), this.toUByte())

val output = mutableListOf<UByte>()

val labels = mutableListOf<Label>()
val labelUses = mutableListOf<LabelUse>()

while(true) {
    val line = readLine()

    if(line != null && line.startsWith(":")) {
        labels.add(Label(line.substringAfter(":"), output.size))
        continue
    }

    val cmd = line?.substringBefore(" ")
    if (cmd == null) break
    val opt = Instruction.valueOf(cmd)
    output.add(opt.code)

    if(!line.contains(" ")) continue
    val args = line?.substringAfter(" ")
    if(args.isNullOrBlank()) continue
    var arg1 = args?.substringBefore(",")
    if(arg1.isNullOrBlank()) continue
    if(arg1.startsWith("@")) {
        labelUses.add(LabelUse(arg1.substringAfter("@"), output.size))
        arg1 = "0"
    }
    output.addAll(arg1.toUInt().toUBytes())
    if(!args.contains(",")) continue
    var arg2 = args?.substringAfterLast(",")
    if(arg2.isNullOrBlank()) continue
    if(arg2.startsWith("@")) {
        labelUses.add(LabelUse(arg2.substringAfter("@"), output.size))
        arg2 = "0"
    }
    output.addAll(arg2.toUInt().toUBytes())
}

labelUses.forEach { use ->
    val label = labels.find { use.name == it.name } ?: return@forEach 
    val locationBytes = label.location.toUInt().toUBytes()
    output[use.location] = locationBytes[0]
    output[use.location + 1] = locationBytes[1]
    output[use.location + 2] = locationBytes[2]
    output[use.location + 3] = locationBytes[3]
}

File("prgm.bin").writeBytes(output.toUByteArray().toByteArray())
