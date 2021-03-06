package com.mokylin.bleach.gameserver.chat.cmd;

import java.util.List;

import com.mokylin.bleach.common.human.HumanPropId;
import com.mokylin.bleach.gameserver.chat.cmd.core.IGmCmdFunction;
import com.mokylin.bleach.gameserver.core.global.ServerGlobals;
import com.mokylin.bleach.gameserver.human.Human;
import com.mokylin.bleach.protobuf.ChatMessage.GCGmCmd;
import com.mokylin.bleach.protobuf.ChatMessage.GCGmCmd.Builder;

/**
 * 消耗体力的GM命令。<p>
 * 
 * 格式：recoverenergy energy[正整数]
 * 
 * @author ChangXiao
 *
 */
public class RecoverEnergyCmdFunc implements IGmCmdFunction {

	@Override
	public String getGmCmd() {
		return "recoverenergy";
	}

	@Override
	public void handle(List<String> paramList, Human human,
			ServerGlobals sGlobals) {
		Builder builder = GCGmCmd.newBuilder();
		String rtnMsg;
		if (paramList != null && paramList.size() > 0) {
			String energy = paramList.get(0);
			int rtn = human.getEnergyManager().recoverEnergy(Integer.parseInt(energy));
			rtnMsg = String.format("恢复体力：%d，当前体力：%d", rtn, human.getInt(HumanPropId.ENERGY));
		} else {
			rtnMsg = "参数有误，格式：recoverenergy energy[正整数]";
		}
		builder.addParam(rtnMsg);
		human.sendMessage(builder);
		
	}

}
