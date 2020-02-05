; ModuleID = 'mwe'
target datalayout = "e-m:e-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-unknown-linux-gnu"

define i1 @bodytrack(float %vr.0, i8** nocapture readonly %vr.2, i32* nocapture %error.in, i32* nocapture %samplePoints.in, i64* nocapture readonly %vr.3, i32 %j.03.in, float %delta2.02.in, float %vr.4, float %vr.5, float %vr.6, float %vr.7, i8* nocapture readnone, <{ float, i32 }>* nocapture) !UID !0 {
my_.lr.ph:
  %2 = fmul float %delta2.02.in, %vr.4, !UID !1
  %3 = fadd float %2, %vr.5, !UID !2
  %4 = fmul float %delta2.02.in, %vr.6, !UID !3
  %5 = fadd float %4, %vr.7, !UID !4
  %6 = fadd float %3, 5.000000e-01, !UID !5
  %7 = fptosi float %6 to i32, !UID !6
  %8 = fadd float %5, 5.000000e-01, !UID !7
  %9 = fptosi float %8 to i32, !UID !8
  %10 = load i64, i64* %vr.3, align 8, !UID !11
  %11 = trunc i64 %10 to i32, !UID !12
  %12 = mul nsw i32 %11, %9, !UID !21
  %13 = add nsw i32 %12, %7, !UID !22
  %14 = sdiv i32 %13, 8, !UID !23
  %15 = sext i32 %14 to i64, !UID !24
  %16 = load i8*, i8** %vr.2, align 8, !UID !25
  %17 = getelementptr inbounds i8, i8* %16, i64 %15, !UID !26
  %18 = load i8, i8* %17, align 1, !UID !27
  %19 = zext i8 %18 to i32, !UID !28
  %20 = and i32 %13, 7, !UID !29
  %21 = lshr i32 %19, %20, !UID !30
  %22 = and i32 %21, 1, !UID !31
  %23 = xor i32 %22, 1, !UID !32
  %24 = load i32, i32* %error.in, align 4, !UID !33
  %25 = add nsw i32 %23, %24, !UID !34
  store i32 %25, i32* %error.in, align 4, !UID !35
  %26 = load i32, i32* %samplePoints.in, align 4, !UID !36
  %27 = add nsw i32 %26, 1, !UID !37
  store i32 %27, i32* %samplePoints.in, align 4, !UID !38
  %28 = fadd float %vr.0, %delta2.02.in, !UID !39
  %29 = add nuw nsw i32 %j.03.in, 1, !UID !40
  %30 = getelementptr <{ float, i32 }>, <{ float, i32 }>* %1, i64 0, i32 0, !UID !43
  store float %28, float* %30, align 4, !LO !44, !UID !45
  %31 = getelementptr <{ float, i32 }>, <{ float, i32 }>* %1, i64 0, i32 1, !UID !46
  store i32 %29, i32* %31, align 4, !LO !44, !UID !47
  ret i1 true, !UID !48, !BB_UID !49
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
!23 = !{!"24"}
!24 = !{!"25"}
!25 = !{!"26"}
!26 = !{!"27"}
!27 = !{!"28"}
!28 = !{!"29"}
!29 = !{!"30"}
!30 = !{!"31"}
!31 = !{!"32"}
!32 = !{!"33"}
!33 = !{!"34"}
!34 = !{!"35"}
!35 = !{!"36"}
!36 = !{!"37"}
!37 = !{!"38"}
!38 = !{!"39"}
!39 = !{!"40"}
!40 = !{!"41"}
!41 = !{!"42"}
!42 = !{!"43"}
!43 = !{!"44"}
!44 = !{!"true"}
!45 = !{!"45"}
!46 = !{!"46"}
!47 = !{!"47"}
!48 = !{!"48"}
!49 = !{!"1"}
