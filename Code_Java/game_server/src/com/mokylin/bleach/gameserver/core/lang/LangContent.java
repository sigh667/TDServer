package com.mokylin.bleach.gameserver.core.lang;

import java.lang.annotation.ElementType;
import java.lang.annotation.Retention;
import java.lang.annotation.RetentionPolicy;
import java.lang.annotation.Target;

/**
 * 语言内容
 * 
 * @author yaguang.xiao
 *
 */
@Retention(RetentionPolicy.RUNTIME)
@Target(ElementType.FIELD)
public @interface LangContent {

	/**
	 * 内容
	 * @return
	 */
	String value() default "";
}
