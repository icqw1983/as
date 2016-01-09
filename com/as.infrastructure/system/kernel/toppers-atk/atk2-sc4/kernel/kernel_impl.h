/*
 *  TOPPERS ATK2
 *      Toyohashi Open Platform for Embedded Real-Time Systems
 *      Automotive Kernel Version 2
 *
 *  Copyright (C) 2011-2015 by Center for Embedded Computing Systems
 *              Graduate School of Information Science, Nagoya Univ., JAPAN
 *  Copyright (C) 2011-2015 by FUJI SOFT INCORPORATED, JAPAN
 *  Copyright (C) 2011-2013 by Spansion LLC, USA
 *  Copyright (C) 2011-2015 by NEC Communication Systems, Ltd., JAPAN
 *  Copyright (C) 2011-2015 by Panasonic Advanced Technology Development Co., Ltd., JAPAN
 *  Copyright (C) 2011-2014 by Renesas Electronics Corporation, JAPAN
 *  Copyright (C) 2011-2015 by Sunny Giken Inc., JAPAN
 *  Copyright (C) 2011-2015 by TOSHIBA CORPORATION, JAPAN
 *  Copyright (C) 2011-2015 by Witz Corporation
 *  Copyright (C) 2014-2015 by AISIN COMCRUISE Co., Ltd., JAPAN
 *  Copyright (C) 2014-2015 by eSOL Co.,Ltd., JAPAN
 *  Copyright (C) 2014-2015 by SCSK Corporation, JAPAN
 *  Copyright (C) 2015 by SUZUKI MOTOR CORPORATION
 *  Copyright (C) 2016 by Fan Wang(parai@foxmail.com), China
 * 
 * The above copyright holders grant permission gratis to use,
 * duplicate, modify, or redistribute (hereafter called use) this
 * software (including the one made by modifying this software),
 * provided that the following four conditions (1) through (4) are
 * satisfied.
 * 
 * (1) When this software is used in the form of source code, the above
 *    copyright notice, this use conditions, and the disclaimer shown
 *    below must be retained in the source code without modification.
 *
 * (2) When this software is redistributed in the forms usable for the
 *    development of other software, such as in library form, the above
 *    copyright notice, this use conditions, and the disclaimer shown
 *    below must be shown without modification in the document provided
 *    with the redistributed software, such as the user manual.
 *
 * (3) When this software is redistributed in the forms unusable for the
 *    development of other software, such as the case when the software
 *    is embedded in a piece of equipment, either of the following two
 *    conditions must be satisfied:
 *
 *  (a) The above copyright notice, this use conditions, and the
 *      disclaimer shown below must be shown without modification in
 *      the document provided with the redistributed software, such as
 *      the user manual.
 *
 *  (b) How the software is to be redistributed must be reported to the
 *      TOPPERS Project according to the procedure described
 *      separately.
 *
 * (4) The above copyright holders and the TOPPERS Project are exempt
 *    from responsibility for any type of damage directly or indirectly
 *    caused from the use of this software and are indemnified by any
 *    users or end users of this software from any and all causes of
 *    action whatsoever.
 *
 * THIS SOFTWARE IS PROVIDED "AS IS." THE ABOVE COPYRIGHT HOLDERS AND
 * THE TOPPERS PROJECT DISCLAIM ANY EXPRESS OR IMPLIED WARRANTIES,
 * INCLUDING, BUT NOT LIMITED TO, ITS APPLICABILITY TO A PARTICULAR
 * PURPOSE. IN NO EVENT SHALL THE ABOVE COPYRIGHT HOLDERS AND THE
 * TOPPERS PROJECT BE LIABLE FOR ANY TYPE OF DAMAGE DIRECTLY OR
 * INDIRECTLY CAUSED FROM THE USE OF THIS SOFTWARE.
 *
 *  $Id: kernel_impl.h 504 2015-12-24 01:22:56Z witz-itoyo $
 */

/*
 *		ATK2内部向け標準ヘッダファイル
 *
 *  このヘッダファイルは，カーネルを構成するプログラムのソースファイル
 *  で必ずインクルードするべき標準ヘッダファイルである
 *
 *  アセンブリ言語のソースファイルからこのファイルをインクルードする時
 *  は，TOPPERS_MACRO_ONLYを定義しておく
 *  これにより，マクロ定義以外を除くようになっている
 */

#ifndef TOPPERS_KERNEL_IMPL_H
#define TOPPERS_KERNEL_IMPL_H

/*
 *  カーネル内部用リネームを使用するための定義
 */
#ifndef TOPPERS_SVC_FUNCCALL
#define TOPPERS_SVC_FUNCCALL
#endif

/*
 *  アプリケーションと共通のヘッダファイル
 */
#define OMIT_INCLUDE_OS_LCFG
#include "Os.h"

/* 無効ポインタ */
#ifndef NULL
#define NULL		NULL_PTR
#endif /* NULL */

/*
 *  型キャストを行うマクロの定義
 */
#ifndef CAST
#define CAST(type, val)		((type) (val))
#endif /* CAST */

#ifndef TOPPERS_MACRO_ONLY

/* 最適化するため，依存部再定義できる型 */
#ifndef OMIT_DATA_TYPE
/*
 *  カーネル内部のデータ型
 */
typedef uint32	InterruptNumberType;            /* 割込み番号 */
typedef uint32	AttributeType;                  /* オブジェクトの属性 */
typedef sint32	PriorityType;                   /* 優先度 */
#endif /* OMIT_DATA_TYPE */

typedef void (*FunctionRefType)(void);          /* プログラムの起動番地 */

/*
 *  エラーフックOFF時，サービスID取得とパラメータ取得もOFFになる
 */
#ifdef CFG_USE_ERRORHOOK

#ifdef CFG_USE_PARAMETERACCESS
extern _ErrorHook_Par	_errorhook_par1;
extern _ErrorHook_Par	_errorhook_par2;
extern _ErrorHook_Par	_errorhook_par3;
#endif /* CFG_USE_PARAMETERACCESS */

#endif /* CFG_USE_ERRORHOOK */

/*
 *  OS内部からのShutdownOSの呼び出し
 */
extern void internal_shutdownos(StatusType ercd);

#ifdef CFG_USE_SHUTDOWNHOOK
extern void internal_call_shtdwnhk(StatusType ercd);
#endif /* CFG_USE_SHUTDOWNHOOK */

#endif /* TOPPERS_MACRO_ONLY */

/*
 *  ASSERTマクロ
 */
#ifndef NDEBUG
#define ASSERT(exp) do {								\
		if (!(exp)) {									\
			fatal_file_name = __FILE__;					\
			fatal_line_num = __LINE__;					\
			internal_shutdownos(E_OS_SYS_ASSERT_FATAL);	\
		}												\
} while (0)

#define ASSERT_NO_REACHED do {						\
		fatal_file_name = __FILE__;					\
		fatal_line_num = __LINE__;					\
		internal_shutdownos(E_OS_SYS_ASSERT_FATAL);	\
} while (0)

#else /* NDEBUG */
#define ASSERT(exp)
#define ASSERT_NO_REACHED
#endif /* NDEBUG */

/*
 *  すべての関数をコンパイルするための定義
 */
#ifdef ALLFUNC
#include "allfunc.h"
#endif /* ALLFUNC */

/*
 *  アプリケーションモード値の定義
 */
#define APPMODE_NONE	((AppModeType) 0)   /* モードなし */

/*
 *  実行中のコンテキスト（callevel_statの下位12ビット）の値の定義
 *  TCL_NULLの時に，本来の呼出下コンテキストが判別できなくなることに注意
 */
#define TCL_NULL			UINT_C(0x0000)                          /* システムサービスを呼び出せない */
#define TCL_TASK			UINT_C(0x0001)                          /* タスク */
#define TCL_ISR2			UINT_C(0x0002)                          /* C2ISR */
#define TCL_PROTECT			UINT_C(0x0004)                          /* ProtectionHook */
#define TCL_PREPOST			UINT_C(0x0008)                          /* PreTaskHook，PostTaskHook */
#define TCL_STARTUP			UINT_C(0x0010)                          /* StartupHook */
#define TCL_SHUTDOWN		UINT_C(0x0020)                          /* ShutdownHook */
#define TCL_ERROR			UINT_C(0x0040)                          /* ErrorHook */
/* OSアプリケーションに所属のフックマスク */
#define TCLMASK				UINT_C(0x0fff)                          /* コールレベルを示すビットのマスク */

/*
 *  システム状態 (callevel_statの上位4ビット)の値の定義
 */
#define TSYS_NULL			UINT_C(0x0000)      /* システム状態クリア */
#define TSYS_DISALLINT		UINT_C(0x1000)      /* DisableAllInterrupts発行中 */
#define TSYS_SUSALLINT		UINT_C(0x2000)      /* SuspendAllInterrupts発行中 */
#define TSYS_SUSOSINT		UINT_C(0x4000)      /* SuspendOSInterrupts発行中 */
#define TSYS_ISR1			UINT_C(0x8000)      /* C1ISR起動済み */
#define TSYSMASK			UINT_C(0xf000)      /* システム状態を示すビットのマスク */


#ifdef CFG_USE_STACKMONITORING
#ifndef STACK_MAGIC_NUMBER
/*
 *  スタックモニタリング用マジックナンバーの定義
 *  ターゲット依存部の定義は優先される
 */
#define STACK_MAGIC_NUMBER	0x4E434553      /* NCESのASCIIコード(0x4E434553) */
#endif /* STACK_MAGIC_NUMBER */

#ifndef TOPPERS_ISTK_MAGIC_REGION
/* 割込みスタック用マジックナンバー領域取得マクロ */
#define TOPPERS_ISTK_MAGIC_REGION(stk, stksz)	(stk)
#endif /* TOPPERS_ISTK_MAGIC_REGION */

#ifndef TOPPERS_SSTK_MAGIC_REGION
/* 信頼タスクスタック用マジックナンバー領域取得マクロ */
#ifndef USE_TSKINICTXB
#define TOPPERS_SSTK_MAGIC_REGION(p_tinib)	((StackType *) ((p_tinib)->sstk))
#endif /* USE_TSKINICTXB */
#endif /* TOPPERS_SSTK_MAGIC_REGION */

#endif /* CFG_USE_STACKMONITORING */

#define TFN_EXIT_TASK	(TMAX_SVCID - TARGET_SVC_NUM)
#define exit_task		(_kernel_exit_task)

/*
 *  フック種別の定義
 */
#define STARTUP_HOOK	UINT_C(0)
#define SHUTDOWN_HOOK	UINT_C(1)
#define ERROR_HOOK		UINT_C(2)

/*
 *  callevel_statのビット操作
 */
#define ENTER_CALLEVEL(bit)		(callevel_stat |= (bit))
#define LEAVE_CALLEVEL(bit)		(callevel_stat &= (uint16) ~((uint32) bit))


/*
 *  各システムサービスを呼び出せる処理単位
 */
#define CALLEVEL_ACTIVATETASK				(TCL_TASK | TCL_ISR2)
#define CALLEVEL_TERMINATETASK				(TCL_TASK)
#define CALLEVEL_CHAINTASK					(TCL_TASK)
#define CALLEVEL_SCHEDULE					(TCL_TASK)
#define CALLEVEL_GETTASKID					(TSYS_DISALLINT | TSYS_SUSALLINT | TSYS_SUSOSINT | TCL_TASK | TCL_ISR2 | TCL_ERROR | TCL_PREPOST | TCL_PROTECT)
#define CALLEVEL_GETTASKSTATE				(TSYS_DISALLINT | TSYS_SUSALLINT | TSYS_SUSOSINT | TCL_TASK | TCL_ISR2 | TCL_ERROR | TCL_PREPOST)
#define CALLEVEL_GETRESOURCE				(TCL_TASK | TCL_ISR2)
#define CALLEVEL_RELEASERESOURCE			(TCL_TASK | TCL_ISR2)
#define CALLEVEL_SETEVENT					(TCL_TASK | TCL_ISR2)
#define CALLEVEL_CLEAREVENT					(TCL_TASK)
#define CALLEVEL_GETEVENT					(TCL_TASK | TCL_ISR2 | TCL_ERROR | TCL_PREPOST)
#define CALLEVEL_WAITEVENT					(TCL_TASK)
#define CALLEVEL_GETALARMBASE				(TCL_TASK | TCL_ISR2 | TCL_ERROR | TCL_PREPOST)
#define CALLEVEL_GETALARM					(TCL_TASK | TCL_ISR2 | TCL_ERROR | TCL_PREPOST)
#define CALLEVEL_SETRELALARM				(TCL_TASK | TCL_ISR2)
#define CALLEVEL_SETABSALARM				(TCL_TASK | TCL_ISR2)
#define CALLEVEL_CANCELALARM				(TCL_TASK | TCL_ISR2)
#define CALLEVEL_GETACTIVEAPPMODE			(TCL_TASK | TCL_ISR2 | TCL_ERROR | TCL_PREPOST | TCL_STARTUP | TCL_SHUTDOWN)
#define CALLEVEL_SHUTDOWNOS					(TCL_TASK | TCL_ISR2 | TCL_ERROR | TCL_STARTUP)
#define CALLEVEL_GETISRID					(TSYS_DISALLINT | TSYS_SUSALLINT | TSYS_SUSOSINT | TCL_TASK | TCL_ISR2 | TCL_ERROR | TCL_PROTECT)
#define CALLEVEL_INCREMENTCOUNTER			(TCL_TASK | TCL_ISR2)
#define CALLEVEL_GETCOUNTERVALUE			(TCL_TASK | TCL_ISR2)
#define CALLEVEL_GETELAPSEDVALUE			(TCL_TASK | TCL_ISR2)
#define CALLEVEL_STARTSCHEDULETABLEREL		(TCL_TASK | TCL_ISR2)
#define CALLEVEL_STARTSCHEDULETABLEABS		(TCL_TASK | TCL_ISR2)
#define CALLEVEL_STOPSCHEDULETABLE			(TCL_TASK | TCL_ISR2)
#define CALLEVEL_NEXTSCHEDULETABLE			(TCL_TASK | TCL_ISR2)
#define CALLEVEL_GETSCHEDULETABLESTATUS		(TCL_TASK | TCL_ISR2)
#define CALLEVEL_GETAPPLICATIONID			(TSYS_DISALLINT | TSYS_SUSALLINT | TSYS_SUSOSINT | TCL_TASK | TCL_ISR2 | TCL_ERROR | TCL_PREPOST | TCL_STARTUP | TCL_SHUTDOWN | TCL_PROTECT)
#define CALLEVEL_CALLTRUSTEDFUNCTION		(TCL_TASK | TCL_ISR2)
#define CALLEVEL_CHECKISRMEMORYACCESS		(TCL_TASK | TCL_ISR2 | TCL_ERROR | TCL_PROTECT)
#define CALLEVEL_CHECKTASKMEMORYACCESS		(TCL_TASK | TCL_ISR2 | TCL_ERROR | TCL_PROTECT)
#define CALLEVEL_CHECKOBJECTACCESS			(TCL_TASK | TCL_ISR2 | TCL_ERROR | TCL_PROTECT)
#define CALLEVEL_CHECKOBJECTOWNERSHIP		(TCL_TASK | TCL_ISR2 | TCL_ERROR | TCL_PROTECT)
#define CALLEVEL_GETAPPLICATIONSTATE		(TSYS_DISALLINT | TSYS_SUSALLINT | TSYS_SUSOSINT | TCL_TASK | TCL_ISR2 | TCL_ERROR | TCL_PREPOST | TCL_STARTUP | TCL_SHUTDOWN | TCL_PROTECT)
#define CALLEVEL_DISABLEINTERRUPTSOURCE		(TCL_TASK | TCL_ISR2 | TCL_ERROR | TCL_PREPOST | TCL_STARTUP | TCL_SHUTDOWN | TCL_PROTECT | TSYS_DISALLINT | TSYS_SUSALLINT | TSYS_SUSOSINT)
#define CALLEVEL_ENABLEINTERRUPTSOURCE		(TCL_TASK | TCL_ISR2 | TCL_ERROR | TCL_PREPOST | TCL_STARTUP | TCL_SHUTDOWN | TCL_PROTECT | TSYS_DISALLINT | TSYS_SUSALLINT | TSYS_SUSOSINT)
#define CALLEVEL_GETFAULTYCONTEXT			(TCL_PROTECT)
#define CALLEVEL_IOCSEND					(TCL_TASK | TCL_ISR2)
#define CALLEVEL_IOCWRITE					(TCL_TASK | TCL_ISR2)
#define CALLEVEL_IOCRECEIVE					(TCL_TASK | TCL_ISR2)
#define CALLEVEL_IOCREAD					(TCL_TASK | TCL_ISR2)
#define CALLEVEL_IOCEMPTYQUEUE				(TCL_TASK | TCL_ISR2)
#define CALLEVEL_ALLOWACCESS				(TCL_TASK | TCL_ISR2)
#define CALLEVEL_TERMINATEAPPLICATION		(TCL_TASK | TCL_ISR2)

/*
 *  その他の定数値（標準割込みモデル）
 */
#define TIPM_ENAALL		UINT_C(0)   /* 割込み優先度マスク全解除 */

/*
 *  オブジェクト属性の定義（標準割込みモデル）
 */
#define ENABLE		UINT_C(0x01)
#define DISABLE		UINT_C(0x00)


/*
 *  OS内部用無効なシステムサービスID
 */
#define OSServiceId_Invalid		((OSServiceIdType) 0xff)

/*
 *  ヘッダファイルを持たないモジュールの関数・変数の宣言
 */
#ifndef TOPPERS_MACRO_ONLY

#ifdef TOPPERS_StartOS
/*
 *  アプリケーションモードの数
 */
extern const AppModeType	tnum_appmode;

#endif /* TOPPERS_StartOS */

/*
 *  OS実行制御のための変数（osctl_manage.c）
 */
extern uint16				callevel_stat;  /* 実行中のコンテキスト */
extern AppModeType			appmodeid;      /* アプリケーションモードID */

/*
 *  カーネル動作状態フラグ
 */
extern boolean				kerflg;

/*
 *  特権モードで動作中かを示すフラグ
 */
extern boolean				run_trusted;

extern boolean				pre_protection_supervised;

/*
 *  エラーフック呼び出しのための宣言（osctl.c）
 */
#ifdef CFG_USE_ERRORHOOK
extern void internal_call_errorhook(StatusType ercd, OSServiceIdType svcid);
#endif /* CFG_USE_ERRORHOOK */

/*
 *  ポストタスクフック/プレタスクフック
 *  スタックモニタリング機能の初期化/プロテクションフック呼び出しのための宣言（osctl.c）
 */
#ifdef CFG_USE_POSTTASKHOOK
extern void call_posttaskhook(void);
#endif /* CFG_USE_POSTTASKHOOK */

#ifdef CFG_USE_PRETASKHOOK
extern void call_pretaskhook(void);
#endif /* CFG_USE_PRETASKHOOK */

#ifdef CFG_USE_STACKMONITORING
extern void init_stack_magic_region(void);
#endif /* CFG_USE_STACKMONITORING */

extern void call_protectionhk_main(StatusType protection_error);

/*
 *  各モジュールの初期化（Os_Lcfg.c）
 */
extern void object_initialize(void);

/*
 *  各モジュールの終了処理（Os_Lcfg.c）
 */
extern void object_terminate(void);

/*
 *  非タスクコンテキスト用のスタック領域（Os_Lcfg.c）
 */
extern const MemorySizeType	_ostksz;        /* スタック領域のサイズ（丸めた値） */
extern StackType * const	_ostk;          /* スタック領域の先頭番地 */
#ifdef TOPPERS_OSTKPT
extern StackType * const	_ostkpt;        /* スタックポインタの初期値 */
#endif /* TOPPERS_OSTKPT */

/*
 *  タイミング保護用時間型
 */
typedef struct time_frame_time {
	uint32		tfcount;
	TickType	tftick;
} TFTIME;

#endif /* TOPPERS_MACRO_ONLY */

#endif /* TOPPERS_KERNEL_IMPL_H */