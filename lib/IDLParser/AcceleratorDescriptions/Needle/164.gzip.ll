; ModuleID = 'mwe'
target datalayout = "e-m:e-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-unknown-linux-gnu"

define i1 @gzip(i8* readnone %vr.0, i8* %match.0.in, i8* %scan.1.in, i8* nocapture readnone, <{ i8*, i8* }>* nocapture) !UID !0 {
my___unk__6:
  ; %2 = getelementptr inbounds i8, i8* %scan.1.in, i64 1, !UID !1
  ; %3 = load i8, i8* %2, align 1, !UID !2
  ; %4 = getelementptr inbounds i8, i8* %match.0.in, i64 1, !UID !3
  ; %5 = load i8, i8* %4, align 1, !UID !4
  ; %6 = icmp eq i8 %3, %5, !UID !5
  ; tail call void @__guard_func(i1 %6, i1 true) #0, !UID !6
  ; %7 = getelementptr inbounds i8, i8* %scan.1.in, i64 2, !UID !7
  ; %8 = load i8, i8* %7, align 1, !UID !8
  ; %9 = getelementptr inbounds i8, i8* %match.0.in, i64 2, !UID !9
  ; %10 = load i8, i8* %9, align 1, !UID !10
  ; %11 = icmp eq i8 %8, %10, !UID !11
  ; tail call void @__guard_func(i1 %11, i1 true) #0, !UID !12
  ; %12 = getelementptr inbounds i8, i8* %scan.1.in, i64 3, !UID !13
  ; %13 = load i8, i8* %12, align 1, !UID !14
  ; %14 = getelementptr inbounds i8, i8* %match.0.in, i64 3, !UID !15
  ; %15 = load i8, i8* %14, align 1, !UID !16
  ; %16 = icmp eq i8 %13, %15, !UID !17
  ; tail call void @__guard_func(i1 %16, i1 true) #0, !UID !18
  ; %17 = getelementptr inbounds i8, i8* %scan.1.in, i64 4, !UID !19
  ; %18 = load i8, i8* %17, align 1, !UID !20
  ; %19 = getelementptr inbounds i8, i8* %match.0.in, i64 4, !UID !21
  ; %20 = load i8, i8* %19, align 1, !UID !22
  ; %21 = icmp eq i8 %18, %20, !UID !23
  ; tail call void @__guard_func(i1 %21, i1 true) #0, !UID !24
  ; %22 = getelementptr inbounds i8, i8* %scan.1.in, i64 5, !UID !25
  ; %23 = load i8, i8* %22, align 1, !UID !26
  ; %24 = getelementptr inbounds i8, i8* %match.0.in, i64 5, !UID !27
  ; %25 = load i8, i8* %24, align 1, !UID !28
  ; ; %26 = icmp eq i8 %23, %25, !UID !29
  ; tail call void @__guard_func(i1 %26, i1 true) #0, !UID !30
  ; %27 = getelementptr inbounds i8, i8* %scan.1.in, i64 6, !UID !31
  ; %28 = load i8, i8* %27, align 1, !UID !32
  ; %29 = getelementptr inbounds i8, i8* %match.0.in, i64 6, !UID !33
  ; %30 = load i8, i8* %29, align 1, !UID !34
  ; %31 = icmp eq i8 %28, %30, !UID !35
  ; tail call void @__guard_func(i1 %31, i1 true) #0, !UID !36
  ; %32 = getelementptr inbounds i8, i8* %scan.1.in, i64 7, !UID !37
  ; %33 = load i8, i8* %32, align 1, !UID !38
  ; %34 = getelementptr inbounds i8, i8* %match.0.in, i64 7, !UID !39
  ; %35 = load i8, i8* %34, align 1, !UID !40
  ; %36 = icmp eq i8 %33, %35, !UID !41
  ; tail call void @__guard_func(i1 %36, i1 true) #0, !UID !42
  %2 = getelementptr inbounds i8, i8* %scan.1.in, i64 8, !UID !43
  ; %38 = load i8, i8* %37, align 1, !UID !44
  %3 = getelementptr inbounds i8, i8* %match.0.in, i64 8, !UID !45
  ; %40 = load i8, i8* %39, align 1, !UID !46
  ; %41 = icmp eq i8 %38, %40, !UID !47
  ; %42 = icmp ult i8* %37, %vr.0, !UID !48
  ; %43 = and i1 %42, %41, !UID !49
  ; tail call void @__guard_func(i1 %43, i1 true) #0, !UID !50
  %4 = getelementptr <{ i8*, i8* }>, <{ i8*, i8* }>* %1, i64 0, i32 0, !UID !51
  store i8* %2, i8** %4, align 8, !LO !52, !UID !53
  %5 = getelementptr <{ i8*, i8* }>, <{ i8*, i8* }>* %1, i64 0, i32 1, !UID !54
  store i8* %3, i8** %5, align 8, !LO !52, !UID !55
  ret i1 true, !UID !56, !BB_UID !57
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
!52 = !{!"true"}
!53 = !{!"53"}
!54 = !{!"54"}
!55 = !{!"55"}
!56 = !{!"56"}
!57 = !{!"1"}
