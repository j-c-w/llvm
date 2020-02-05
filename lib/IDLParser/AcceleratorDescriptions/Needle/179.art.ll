; ModuleID = 'mwe'
target datalayout = "e-m:e-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-unknown-linux-gnu"

%struct.xyz = type { double, i32 }
%struct.f1_neuron = type { double*, double, double, double, double, double, double, double }

define i1 @art(i64 %indvars.iv.next84135.in, i64 %indvars.iv85.in, %struct.xyz** nocapture readonly %Y.in, %struct.f1_neuron** nocapture readonly %f1_layer.in, double*** nocapture readonly %bus.in, i32* nocapture readonly %numf1s.in, i8* nocapture readnone, <{ i64 }>* nocapture) !UID !0 {
my_.lr.ph48..lr.ph48_crit_edge:
  %2 = load %struct.xyz*, %struct.xyz** %Y.in, align 8, !UID !1
  %3 = load %struct.f1_neuron*, %struct.f1_neuron** %f1_layer.in, align 8, !UID !2
  %4 = getelementptr inbounds %struct.f1_neuron, %struct.f1_neuron* %3, i64 %indvars.iv.next84135.in, i32 5, !UID !3
  %5 = load double, double* %4, align 8, !UID !4
  %6 = load double**, double*** %bus.in, align 8, !UID !5
  %7 = getelementptr inbounds double*, double** %6, i64 %indvars.iv.next84135.in, !UID !6
  %8 = load double*, double** %7, align 8, !UID !7
  %9 = getelementptr inbounds double, double* %8, i64 %indvars.iv85.in, !UID !8
  %10 = load double, double* %9, align 8, !UID !9
  %11 = fmul double %5, %10, !UID !10
  %12 = getelementptr inbounds %struct.xyz, %struct.xyz* %2, i64 %indvars.iv85.in, i32 0, !UID !11
  %13 = load double, double* %12, align 8, !UID !12
  %14 = fadd double %11, %13, !UID !13
  store double %14, double* %12, align 8, !UID !14
  %15 = add nuw nsw i64 %indvars.iv.next84135.in, 1, !UID !15
  %16 = getelementptr <{ i64 }>, <{ i64 }>* %1, i64 0, i32 0, !UID !20
  store i64 %15, i64* %16, align 8, !LO !21, !UID !22
  ret i1 true, !UID !23, !BB_UID !24
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
!21 = !{!"true"}
!22 = !{!"22"}
!23 = !{!"23"}
!24 = !{!"1"}
