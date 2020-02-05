; ModuleID = 'mwe'
target datalayout = "e-m:e-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-unknown-linux-gnu"

define i1 @hmmer(i32 %vr.0, i64 %indvars.iv.in, i32* nocapture readonly %vr.1, i32* nocapture readonly %vr.2, i32* nocapture %vr.3, i32* nocapture readonly %vr.4, i32* nocapture readonly %vr.5, i32* nocapture readonly %vr.6, i32* nocapture readonly %vr.7, i32* nocapture readonly %vr.8, i32 %vr.9, i32* nocapture readonly %vr.10, i32* nocapture %vr.11, i32* nocapture readonly %vr.12, i32* nocapture readonly %vr.13, i64 %vr.14, i8* nocapture readnone, <{ i64 }>* nocapture) !UID !0 {
my_.lr.ph:
  %2 = add nsw i64 %indvars.iv.in, -1, !UID !1
  %3 = getelementptr inbounds i32, i32* %vr.1, i64 %2, !UID !2
  %4 = load i32, i32* %3, align 4, !UID !3
  %5 = getelementptr inbounds i32, i32* %vr.2, i64 %2, !UID !4
  %6 = load i32, i32* %5, align 4, !UID !5
  %7 = add nsw i32 %6, %4, !UID !6
  %8 = getelementptr inbounds i32, i32* %vr.3, i64 %indvars.iv.in, !UID !7
  store i32 %7, i32* %8, align 4, !UID !8
  %9 = getelementptr inbounds i32, i32* %vr.4, i64 %2, !UID !9
  %10 = load i32, i32* %9, align 4, !UID !10
  %11 = getelementptr inbounds i32, i32* %vr.5, i64 %2, !UID !11
  %12 = load i32, i32* %11, align 4, !UID !12
  %13 = add nsw i32 %12, %10, !UID !13
  %14 = icmp sgt i32 %13, %7, !UID !14
  %15 = select i1 %14, i32 %13, i32 %7, !UID !15
  store i32 %15, i32* %8, align 4, !UID !16
  %16 = getelementptr inbounds i32, i32* %vr.6, i64 %2, !UID !17
  %17 = load i32, i32* %16, align 4, !UID !18
  %18 = getelementptr inbounds i32, i32* %vr.7, i64 %2, !UID !19
  %19 = load i32, i32* %18, align 4, !UID !20
  %20 = add nsw i32 %19, %17, !UID !21
  %21 = icmp sgt i32 %20, %15, !UID !22
  %22 = select i1 %21, i32 %20, i32 %15, !UID !23
  store i32 %22, i32* %8, align 4, !UID !24
  %23 = getelementptr inbounds i32, i32* %vr.8, i64 %indvars.iv.in, !UID !25
  %24 = load i32, i32* %23, align 4, !UID !26
  %25 = add nsw i32 %24, %vr.9, !UID !27
  %26 = icmp sgt i32 %25, %22, !UID !28
  %27 = select i1 %26, i32 %25, i32 %22, !UID !29
  store i32 %27, i32* %8, align 4, !UID !30
  %28 = getelementptr inbounds i32, i32* %vr.10, i64 %indvars.iv.in, !UID !31
  %29 = load i32, i32* %28, align 4, !UID !32
  %30 = add nsw i32 %27, %29, !UID !33
  %31 = icmp slt i32 %30, -987654321, !UID !34
  %32 = select i1 %31, i32 -987654321, i32 %30, !UID !35
  store i32 %32, i32* %8, align 4, !UID !36
  %33 = getelementptr inbounds i32, i32* %vr.11, i64 %2, !UID !37
  %34 = load i32, i32* %33, align 4, !UID !38
  %35 = getelementptr inbounds i32, i32* %vr.12, i64 %2, !UID !39
  %36 = load i32, i32* %35, align 4, !UID !40
  %37 = add nsw i32 %36, %34, !UID !41
  %38 = getelementptr inbounds i32, i32* %vr.11, i64 %indvars.iv.in, !UID !42
  store i32 %37, i32* %38, align 4, !UID !43
  %39 = getelementptr inbounds i32, i32* %vr.3, i64 %2, !UID !44
  %40 = load i32, i32* %39, align 4, !UID !45
  %41 = getelementptr inbounds i32, i32* %vr.13, i64 %2, !UID !46
  %42 = load i32, i32* %41, align 4, !UID !47
  %43 = add nsw i32 %42, %40, !UID !48
  %44 = icmp sgt i32 %43, %37, !UID !49
  %45 = select i1 %44, i32 %43, i32 %37, !UID !50
  %46 = icmp slt i32 %45, -987654321, !UID !51
  %47 = select i1 %46, i32 -987654321, i32 %45, !UID !52
  store i32 %47, i32* %38, align 4, !UID !53
  %48 = add nuw nsw i64 %indvars.iv.in, 1, !UID !56
  %49 = getelementptr <{ i64 }>, <{ i64 }>* %1, i64 0, i32 0, !UID !60
  store i64 %48, i64* %49, align 8, !LO !61, !UID !62
  ret i1 true, !UID !63, !BB_UID !64
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
!44 = !{!"45"}
!45 = !{!"46"}
!46 = !{!"47"}
!47 = !{!"48"}
!48 = !{!"49"}
!49 = !{!"50"}
!50 = !{!"51"}
!51 = !{!"52"}
!52 = !{!"53"}
!53 = !{!"54"}
!54 = !{!"55"}
!55 = !{!"56"}
!56 = !{!"57"}
!57 = !{!"58"}
!58 = !{!"59"}
!59 = !{!"60"}
!60 = !{!"61"}
!61 = !{!"true"}
!62 = !{!"62"}
!63 = !{!"63"}
!64 = !{!"1"}
