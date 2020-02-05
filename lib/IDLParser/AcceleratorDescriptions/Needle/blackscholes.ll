; ModuleID = 'mwe'
target datalayout = "e-m:e-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-unknown-linux-gnu"

define i1 @blackscholes(i32 %ix.348.in.i.i.in, i32 %r.047.i.i.in, i32 %q.046.i.i.in, i32 %s.045.i.i.in, i8* nocapture readnone, <{ i32, i32, i32, i32 }>* nocapture) !UID !0 {
my___unk__7:
  %2 = shl nsw i32 %ix.348.in.i.i.in, 1, !UID !1
  %3 = lshr i32 %r.047.i.i.in, 1, !UID !5
  %4 = getelementptr <{ i32, i32, i32, i32 }>, <{ i32, i32, i32, i32 }>* %1, i64 0, i32 0, !UID !8
  store i32 %2, i32* %4, align 4, !LO !9, !UID !10
  %5 = getelementptr <{ i32, i32, i32, i32 }>, <{ i32, i32, i32, i32 }>* %1, i64 0, i32 1, !UID !11
  store i32 %s.045.i.i.in, i32* %5, align 4, !LO !9, !UID !12
  %6 = getelementptr <{ i32, i32, i32, i32 }>, <{ i32, i32, i32, i32 }>* %1, i64 0, i32 2, !UID !13
  store i32 %q.046.i.i.in, i32* %6, align 4, !LO !9, !UID !14
  %7 = getelementptr <{ i32, i32, i32, i32 }>, <{ i32, i32, i32, i32 }>* %1, i64 0, i32 3, !UID !15
  store i32 %3, i32* %7, align 4, !LO !9, !UID !16
  ret i1 true, !UID !17, !BB_UID !18
}


attributes #0 = { noinline readnone }

!0 = !{!"0"}
!1 = !{!"2"}
!2 = !{!"3"}
!3 = !{!"4"}
!4 = !{!"5"}
!5 = !{!"6"}
!6 = !{!"7"}
!7 = !{!"8"}
!8 = !{!"9"}
!9 = !{!"true"}
!10 = !{!"10"}
!11 = !{!"11"}
!12 = !{!"12"}
!13 = !{!"13"}
!14 = !{!"14"}
!15 = !{!"15"}
!16 = !{!"16"}
!17 = !{!"17"}
!18 = !{!"1"}
