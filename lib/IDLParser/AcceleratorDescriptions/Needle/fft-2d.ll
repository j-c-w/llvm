; ModuleID = 'mwe'
target datalayout = "e-m:e-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-unknown-linux-gnu"

define i1 @fft2d(i32 %b.02.in, i32 %a.05.in, i32 %transform_length.09.in, float* nocapture %data.in, float %w_real.03.in, float %w_imag.04.in, i32 %vr.0, i32 %N.in, i8* nocapture readnone, <{ i32 }>* nocapture) !UID !0 {
my_.lr.ph:
  %2 = add i32 %a.05.in, %b.02.in, !UID !1
  %3 = add i32 %2, %transform_length.09.in, !UID !2
  %4 = shl i32 %3, 1, !UID !3
  %5 = zext i32 %4 to i64, !UID !4
  %6 = getelementptr inbounds float, float* %data.in, i64 %5, !UID !5
  %7 = load float, float* %6, align 4, !UID !6
  %8 = or i32 %4, 1, !UID !7
  %9 = zext i32 %8 to i64, !UID !8
  %10 = getelementptr inbounds float, float* %data.in, i64 %9, !UID !9
  %11 = load float, float* %10, align 4, !UID !10
  %12 = fmul float %7, %w_real.03.in, !UID !11
  %13 = fmul float %11, %w_imag.04.in, !UID !12
  %14 = fsub float %12, %13, !UID !13
  %15 = fmul float %11, %w_real.03.in, !UID !14
  %16 = fmul float %7, %w_imag.04.in, !UID !15
  %17 = fadd float %16, %15, !UID !16
  %18 = shl i32 %2, 1, !UID !17
  %19 = zext i32 %18 to i64, !UID !18
  %20 = getelementptr inbounds float, float* %data.in, i64 %19, !UID !19
  %21 = load float, float* %20, align 4, !UID !20
  %22 = fsub float %21, %14, !UID !21
  store float %22, float* %6, align 4, !UID !22
  %23 = or i32 %18, 1, !UID !23
  %24 = zext i32 %23 to i64, !UID !24
  %25 = getelementptr inbounds float, float* %data.in, i64 %24, !UID !25
  %26 = load float, float* %25, align 4, !UID !26
  %27 = fsub float %26, %17, !UID !27
  store float %27, float* %10, align 4, !UID !28
  %28 = load float, float* %20, align 4, !UID !29
  %29 = fadd float %14, %28, !UID !30
  store float %29, float* %20, align 4, !UID !31
  %30 = load float, float* %25, align 4, !UID !32
  %31 = fadd float %17, %30, !UID !33
  store float %31, float* %25, align 4, !UID !34
  %32 = add i32 %vr.0, %b.02.in, !UID !35
  %33 = getelementptr <{ i32 }>, <{ i32 }>* %1, i64 0, i32 0, !UID !38
  store i32 %32, i32* %33, align 4, !LO !39, !UID !40
  ret i1 true, !UID !41, !BB_UID !42
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
!39 = !{!"true"}
!40 = !{!"40"}
!41 = !{!"41"}
!42 = !{!"1"}
