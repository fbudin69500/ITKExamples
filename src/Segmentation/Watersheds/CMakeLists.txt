
add_example(SegmentWithWatershedImageFilter)
compare_to_baseline(EXAMPLE_NAME SegmentWithWatershedImageFilter
  TEST_NAME SegmentWithWatershedImageFilterTest01BaselineComparison
  BASELINE_PREFIX SegmentWithWatershedImageFilterTest01Baseline
  DEPENDS SegmentWithWatershedImageFilterTest01
  TEST_IMAGE SegmentWithWatershedImageFilterTest01.png
  )
