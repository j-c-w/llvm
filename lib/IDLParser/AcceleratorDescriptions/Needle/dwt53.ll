; ModuleID = 'mwe'
target datalayout = "e-m:e-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-unknown-linux-gnu"

define i1 @__offload_func_0(i64 %indvars.iv3.in, i64 %vr.0, i32* nocapture readonly %data.in, i64 %vr.1, i64 %indvars.iv17.in, i32* nocapture %data2.in, i64 %vr.2, i64 %vr.3, i64 %vr.4, i8* nocapture readnone, <{ i64 }>* nocapture) !UID !0 {
my_.lr.ph8:
  %2 = shl i64 %indvars.iv3.in, 1, !UID !1
  %3 = add nsw i64 %2, %vr.0, !UID !2
  %4 = getelementptr inbounds i32, i32* %data.in, i64 %3, !UID !3
  %5 = load i32, i32* %4, align 4, !UID !4
  %6 = mul nsw i64 %vr.1, %indvars.iv3.in, !UID !5
  %7 = add nsw i64 %6, %indvars.iv17.in, !UID !6
  %8 = getelementptr inbounds i32, i32* %data2.in, i64 %7, !UID !7
  store i32 %5, i32* %8, align 4, !UID !8
  %9 = add i64 %2, %vr.2, !UID !9
  %10 = shl i64 %9, 32, !UID !10
  %11 = ashr exact i64 %10, 32, !UID !11
  %12 = getelementptr inbounds i32, i32* %data.in, i64 %11, !UID !12
  %13 = load i32, i32* %12, align 4, !UID !13
  %14 = add nsw i64 %vr.3, %indvars.iv3.in, !UID !14
  %15 = mul nsw i64 %14, %vr.1, !UID !15
  %16 = add nsw i64 %15, %vr.4, !UID !16
  %17 = getelementptr inbounds i32, i32* %data2.in, i64 %16, !UID !17
  store i32 %13, i32* %17, align 4, !UID !18
  %18 = add nuw nsw i64 %indvars.iv3.in, 1, !UID !19
  %19 = getelementptr <{ i64 }>, <{ i64 }>* %1, i64 0, i32 0, !UID !22
  store i64 %18, i64* %19, align 8, !LO !23, !UID !24
  ret i1 true, !UID !25, !BB_UID !26
}

declare void @__guard_func(i1, i1)

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
!9 = !{!"10"}
!10 = !{!"11"}
!11 = !{!"12"}
!12 = !{!"13"}
!13 = !{!"14"}
!14 = !{!"15"}
!15 = !{!"16"}
!16 = !{!"17"}
!17 = !{!"18"}
!18 = !{!"19"}
!19 = !{!"20"}
!20 = !{!"21"}
!21 = !{!"22"}
!22 = !{!"23"}
!23 = !{!"true"}
!24 = !{!"24"}
!25 = !{!"25"}
!26 = !{!"1"}
