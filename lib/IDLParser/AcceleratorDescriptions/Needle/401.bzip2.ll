; ModuleID = 'mwe'
target datalayout = "e-m:e-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-unknown-linux-gnu"

%struct.EState = type { %struct.bz_stream*, i32, i32, i32, i32*, i32*, i32*, i32, i32*, i8*, i16*, i8*, i32, i32, i32, i32, i32, i32, i32, i32, i32, i32, [256 x i8], [256 x i8], i32, i32, i32, i32, i32, i32, i32, i32, [258 x i32], [18002 x i8], [18002 x i8], [6 x [258 x i8]], [6 x [258 x i32]], [6 x [258 x i32]], [258 x [4 x i32]] }
%struct.bz_stream = type { i8*, i32, i32, i32, i8*, i32, i32, i32, i8*, i8* (i8*, i32, i32)*, void (i8*, i8*)*, i8* }

; Function Attrs: norecurse nounwind
define i1 @bzip2(i32 %zPend.2.i.in, i64 %indvars.iv20.i.in, i8 %vr.0, i8* nocapture %vr.1, i64 %vr.2, i32 %wr.3.i.in, i16* nocapture %vr.3, %struct.EState* nocapture %s.in, i32* nocapture readonly %vr.4, i8* %.lcssa187.in, i8* nocapture readnone, <{ i32, i32, i32, i64, i1 }>* nocapture) #0 !UID !0 {
my_._crit_edge.i.loopexit:
  store i8 %vr.0, i8* %vr.1, align 16, !UID !1
  %2 = ptrtoint i8* %.lcssa187.in to i64, !UID !2
  %3 = sub i64 %2, %vr.2, !UID !3
  %4 = trunc i64 %3 to i32, !UID !4
  %5 = add nsw i32 %4, 1, !UID !5
  %6 = trunc i32 %5 to i16, !UID !6
  %7 = sext i32 %wr.3.i.in to i64, !UID !7
  %8 = getelementptr inbounds i16, i16* %vr.3, i64 %7, !UID !8
  store i16 %6, i16* %8, align 2, !UID !9
  %9 = sext i32 %5 to i64, !UID !10
  %10 = getelementptr inbounds %struct.EState, %struct.EState* %s.in, i64 0, i32 32, i64 %9, !UID !11
  %11 = load i32, i32* %10, align 4, !UID !12
  %12 = add nsw i32 %11, 1, !UID !13
  store i32 %12, i32* %10, align 4, !UID !14
  %13 = load i32, i32* %vr.4, align 4, !UID !15
  %14 = add nsw i32 %wr.3.i.in, 1, !UID !16
  %15 = add nuw nsw i64 %indvars.iv20.i.in, 1, !UID !17
  %16 = sext i32 %13 to i64, !UID !18
  %17 = icmp slt i64 %15, %16, !UID !19
  %18 = getelementptr <{ i32, i32, i32, i64, i1 }>, <{ i32, i32, i32, i64, i1 }>* %1, i64 0, i32 0, !UID !20
  store i32 %13, i32* %18, align 4, !LO !21, !UID !22
  %19 = getelementptr <{ i32, i32, i32, i64, i1 }>, <{ i32, i32, i32, i64, i1 }>* %1, i64 0, i32 1, !UID !23
  store i32 %14, i32* %19, align 4, !LO !21, !UID !24
  %20 = getelementptr <{ i32, i32, i32, i64, i1 }>, <{ i32, i32, i32, i64, i1 }>* %1, i64 0, i32 2, !UID !25
  store i32 %zPend.2.i.in, i32* %20, align 4, !LO !21, !UID !26
  %21 = getelementptr <{ i32, i32, i32, i64, i1 }>, <{ i32, i32, i32, i64, i1 }>* %1, i64 0, i32 3, !UID !27
  store i64 %15, i64* %21, align 8, !LO !21, !UID !28
  %22 = getelementptr <{ i32, i32, i32, i64, i1 }>, <{ i32, i32, i32, i64, i1 }>* %1, i64 0, i32 4, !UID !29
  store i1 %17, i1* %22, align 1, !LO !21, !UID !30
  ret i1 true, !UID !31, !BB_UID !32
}

attributes #0 = { norecurse nounwind }

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
!24 = !{!"24"}
!25 = !{!"25"}
!26 = !{!"26"}
!27 = !{!"27"}
!28 = !{!"28"}
!29 = !{!"29"}
!30 = !{!"30"}
!31 = !{!"31"}
!32 = !{!"1"}
